// Copyright Epic Games, Inc. All Rights Reserved.

#include "TetriTestGameMode.h"
#include <iostream>     // std::cout, std::ostream, std::ios
#include <fstream>      // std::filebuf
#include <map>

#include "Figure.h"
#include "TetriTestHUD.h"
#include "TetriTestCharacter.h"
#include "CubeComponent.h"
#include "CubeActor.h"
#include "UObject/ConstructorHelpers.h"
#include "TetriTestStateBase.h"



#define FIGURES 5 


bool AllFigures[FIGURES][4][2] = {
	{{1,1}, {1,1}, {}, {}},		//square
	{{1}, {1,1}, {1}, {}},		//triangle .|.
	{{1}, {1}, {1,1}, {}},		//seven ..|
	{{1}, {1,1}, {0,1}, {}},	//snake .|'
	{{1}, {1}, {1}, {1}}		//stick
};

ATetriTestGameMode* ATetriTestGameMode::instance = nullptr;
long ATetriTestGameMode::lastID = 0;
bool ATetriTestGameMode::canFigureDrop = true;
int ATetriTestGameMode::sceneHeight = 20;
int ATetriTestGameMode::sceneSize = 4;
const float ATetriTestGameMode::maxHeight = 19000.f;

ATetriTestGameMode::ATetriTestGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD and GameState class
	HUDClass = ATetriTestHUD::StaticClass();
	GameStateClass = ATetriTestStateBase::StaticClass();

	instance = this;

	fullScene = new AActor***[sceneSize];

	//clear scene
	for (int x = 0; x < sceneSize; x++) {
		fullScene[x] = new AActor **[sceneSize];
		for (int y = 0; y < sceneSize; y++) {
			fullScene[x][y] = new AActor *[sceneHeight];
			for (int z = 0; z < sceneHeight; z++)
				fullScene[x][y][z] = nullptr;
		}
	}
	
}

//stop dropping
ATetriTestGameMode::~ATetriTestGameMode() { 
	if (canFigureDrop)
		ClearScene();

	canFigureDrop = false;
	if (fullScene != nullptr)
	{
		for (int x = 0; x < sceneSize; x++) {
			for (int y = 0; y < sceneSize; y++)
				delete[] fullScene[x][y];
			delete[] fullScene[x];
		}
		delete[] fullScene;
	}
		
}


void ATetriTestGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) {
	Super::InitGame(MapName, Options, ErrorMessage);
	instance = this;
}

void ATetriTestGameMode::BeginPlay() {
	Super::BeginPlay();
	DropFigure();
}

void ATetriTestGameMode::ClearScene() {
	for (int x = 0; x < sceneSize; x++) {
		for (int y = 0; y < sceneSize; y++) {
			for (int z = 0; z < sceneHeight; z++) {
				if (fullScene[x][y][z] != nullptr) {
					AActor* actor = fullScene[x][y][z];
					fullScene[x][y][z] = nullptr;
					//could have wrong address;
					actor->Destroy();
				}
			}
		}
	}
}

void ATetriTestGameMode::DropFigure() {
	if (canFigureDrop) {
		UCubeComponent* owner = nullptr;
		if (GetWorld() != nullptr && (fallingFigure == nullptr || fallingFigure->IsItFalling() == false)) {
			AFigure* fig = AFigure::SpawnFigure(GetWorld());
			srand(time(NULL));
			int figure = rand() % 5;
			bool first = true;
			for (int x = 0; x < 4; x++)
				for (int y = 0; y < 2; y++)
					if (AllFigures[figure][x][y]) {
						UCubeComponent::SpawnBlock(x, y, fig, GetNextId(), GetWorld());
					}

			fallingFigure = fig;
		}
	}
}

ATetriTestGameMode* ATetriTestGameMode::GetGameMode() { return instance; }

void ATetriTestGameMode::ClearBlockLocation(AActor* block) {
	if (canFigureDrop) {
		FVector pos = block->GetActorLocation();
		int x, y, z;
		CalcXYZFromPos(pos, x, y, z);
		fullScene[x][y][z] = nullptr;
	}
}

void ATetriTestGameMode::ClearFallingFigure() { fallingFigure = nullptr; }

void ATetriTestGameMode::CalcXYZFromPos(const FVector pos, int& x, int& y, int& z) {
	x = (((pos.X - _BLOCK_SIZE_ / 2.f) / _BLOCK_SIZE_) + sceneSize / 2.f);
	y = (((pos.Y - _BLOCK_SIZE_ / 2.f) / _BLOCK_SIZE_) + sceneSize / 2.f);
	z = ((pos.Z - _BLOCK_SIZE_ / 2.f) / _BLOCK_SIZE_);
	z -= pos.Z < 500 ? 1 : 0;
}

FVector ATetriTestGameMode::CalcPosFromXYZ(const int x, const int y, const int z) {
	FVector pos;
	pos.X = (x - sceneSize / 2.f) * _BLOCK_SIZE_ + _BLOCK_SIZE_ / 2.f;
	pos.Y = (y - sceneSize / 2.f) * _BLOCK_SIZE_ + _BLOCK_SIZE_ / 2.f;
	pos.Z = (z) * _BLOCK_SIZE_ + _BLOCK_SIZE_ / 2.f + 100.f;
	return pos;
}

void ATetriTestGameMode::CheckAndDestroyLayers(AFigure* figure) {
	std::map<int, int> zCoord;
	zCoord.clear();
	figure->GetZCoordinates(zCoord);
	int layers = 0;
	bool stop = false;
	auto iter = zCoord.end();
	do{
		iter--;
		//if there is one block or more in that layer
		int z = iter->first; //it should be key
		bool check = true;
		for (int x = 0; x < sceneSize && check; x++)
			for (int y = 0; y < sceneSize && check; y++)
				check &= fullScene[x][y][z] != nullptr;
		if (check) {
			layers++;
			DestroyLayer(z);
		}
	}while (iter != zCoord.begin());
	int scores = 0;

	switch (layers) {
	case 1: scores = 1; break;
	case 2: scores = 3; break;
	case 3: scores = 5; break;
	case 4: scores = 7; break;
	}
	ATetriTestCharacter::scores += scores;
}

void ATetriTestGameMode::DestroyLayer(int z) {
	for (int x = 0; x < sceneSize; x++)
		for (int y = 0; y < sceneSize; y++)
			((ACubeActor*)fullScene[x][y][z])->owner->DestroyBlock(((ACubeActor*)fullScene[x][y][z])->CubeComp->id);

	//move all layers on one down
	for (int x = 0; x < sceneSize; x++)
		for (int y = 0; y < sceneSize; y++)
			for (int newZ = z + 1; newZ + 1 < sceneHeight; newZ++)
				if (fullScene[x][y][newZ] != nullptr)
					MoveBlockInScene(fullScene[x][y][newZ], fullScene[x][y][newZ]->GetActorLocation(), CalcPosFromXYZ(x, y, newZ - 1));

	//upper layer should be empty
	for (int x = 0; x < sceneSize; x++)
		for (int y = 0; y < sceneSize; y++)
			fullScene[x][y][sceneHeight - 1] = nullptr;

}

FVector ATetriTestGameMode::CorrectPosition(FVector pos, long id) {
	int x, y, z;
	CalcXYZFromPos(pos, x, y, z);
	return CalcPosFromXYZ(x, y, z);
}


bool ATetriTestGameMode::CheckMoveBlock(const FVector newPos, AFigure* owner) {
	return CheckMoveBlockWithID(newPos, owner->GetId());
}

//Is it a free space on a new locations?
bool ATetriTestGameMode::CheckMoveBlockWithID(const FVector newPos, long id) {
	FVector playerPos = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	int x, y, z;
	CalcXYZFromPos(newPos, x, y, z);
	bool check = (x >= 0 && x < sceneSize&& y >= 0 && y < sceneSize&& z >= 0 && z < sceneHeight);			//check scene coordinates
	float dist = newPos.Dist(playerPos, newPos);
	check &= (dist > (_BLOCK_SIZE_ /1.5f));
	if (check) {
		check = check && fullScene[x][y][z] == nullptr;															//check address
		if (check == false) {
			AFigure* coordOwner = ((ACubeActor*)(fullScene[x][y][z]))->owner;
			check = (coordOwner->GetId() == id);												//
			
		}
	}

	if(check == false)
		x = 10;

	return check;
}

bool ATetriTestGameMode::CheckMoveFigure(std::vector<FVector> &figurePos, AFigure* owner) {
	bool check = true;
	for (FVector pos : figurePos)
		check &= CheckMoveBlock(pos, owner);

	//UE_LOG(LogTemp, Warning, TEXT("check: %d"), check ? 1 : 0);
	return check;
}

void ATetriTestGameMode::MoveBlockInScene(AActor* block, const FVector oldPos, const FVector newPos) {
	int x, y, z;
	CalcXYZFromPos(oldPos, x, y, z);
	fullScene[x][y][z] = nullptr;
	CalcXYZFromPos(newPos, x, y, z);
	fullScene[x][y][z] = block;
	block->SetActorLocation(newPos);
}

long ATetriTestGameMode::GetNextId() { return ++lastID; }