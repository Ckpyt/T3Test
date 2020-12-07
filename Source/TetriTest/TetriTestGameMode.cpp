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

ATetriTestGameMode::ATetriTestGameMode()
	: Super()
{
	LStream Stream;
	std::cout.rdbuf(&Stream);
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ATetriTestHUD::StaticClass();


	instance = this;
	//std::cout << "Game mode started!!!" << std::endl;

	//clear scene
	for (int z = 0; z < SCENE_HEIGHT; z++) 
		for (int y = 0; y < SCENE_SIZE; y++) 
			for (int x = 0; x < SCENE_SIZE; x++) 
				fullScene[x][y][z] = nullptr;

	
}

//does not work ;-(
void ATetriTestGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) {
	Super::InitGame(MapName, Options, ErrorMessage);
	instance = this;
}

void ATetriTestGameMode::BeginPlay() {
	Super::BeginPlay();
	DropFigure();
}

void ATetriTestGameMode::ClearScene() {
	for (int z = 0; z < SCENE_HEIGHT; z++) {
		for (int y = 0; y < SCENE_SIZE; y++) {
			for (int x = 0; x < SCENE_SIZE; x++) {
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

ATetriTestGameMode* ATetriTestGameMode::GetGameMode() { return instance; }

void ATetriTestGameMode::ClearBlockLocation(AActor* block) {
	FVector pos = block->GetActorLocation();
	int x, y, z;
	CalcXYZFromPos(pos, x, y, z);
	fullScene[x][y][z] = nullptr;
}

void ATetriTestGameMode::ClearFallingFigure() { fallingFigure = nullptr; }

void ATetriTestGameMode::CalcXYZFromPos(const FVector pos, int& x, int& y, int& z) {
	x = (((pos.X - _BLOCK_SIZE_ / 2.f) / _BLOCK_SIZE_) + SCENE_SIZE / 2.f);
	y = (((pos.Y - _BLOCK_SIZE_ / 2.f) / _BLOCK_SIZE_) + SCENE_SIZE / 2.f);
	z = ((pos.Z - _BLOCK_SIZE_ / 2.f) / _BLOCK_SIZE_);
	z -= pos.Z < 500 ? 1 : 0;
}

FVector ATetriTestGameMode::CalcPosFromXYZ(const int x, const int y, const int z) {
	FVector pos;
	pos.X = (x - SCENE_SIZE / 2.f) * _BLOCK_SIZE_ + _BLOCK_SIZE_ / 2.f;
	pos.Y = (y - SCENE_SIZE / 2.f) * _BLOCK_SIZE_ + _BLOCK_SIZE_ / 2.f;
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
		for (int x = 0; x < SCENE_SIZE && check; x++)
			for (int y = 0; y < SCENE_SIZE && check; y++)
				check &= fullScene[x][y][z] != nullptr;
		if (check) {
			layers++;
			DestroyLayer(z);
		}
	}while (iter != zCoord.begin());
}

void ATetriTestGameMode::DestroyLayer(int z) {
	for (int x = 0; x < SCENE_SIZE; x++)
		for (int y = 0; y < SCENE_SIZE; y++)
			((ACubeActor*)fullScene[x][y][z])->owner->DestroyBlock(((ACubeActor*)fullScene[x][y][z])->CubeComp->id);

	//move all layers on one down
	for (int x = 0; x < SCENE_SIZE; x++)
		for (int y = 0; y < SCENE_SIZE; y++)
			for (int newZ = z + 1; newZ + 1 < SCENE_HEIGHT; newZ++)
				if (fullScene[x][y][newZ] != nullptr)
					MoveBlockInScene(fullScene[x][y][newZ], fullScene[x][y][newZ]->GetActorLocation(), CalcPosFromXYZ(x, y, newZ - 1));

	//upper layer should be empty
	for (int x = 0; x < SCENE_SIZE; x++)
		for (int y = 0; y < SCENE_SIZE; y++)
			fullScene[x][y][SCENE_HEIGHT - 1] = nullptr;

}

FVector ATetriTestGameMode::CorrectPosition(FVector pos) {
	int x, y, z;
	CalcXYZFromPos(pos, x, y, z);
	return CalcPosFromXYZ(x, y, z);
}

//Is it a free space on a new locations?
bool ATetriTestGameMode::CheckMoveBlock(const FVector newPos, AFigure* owner) {
	FVector playerPos = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	int x, y, z;
	CalcXYZFromPos(newPos, x, y, z);
	bool check = (x >= 0 && x < SCENE_SIZE&& y >= 0 && y < SCENE_SIZE&& z >= 0 && z < SCENE_HEIGHT);			//check scene coordinates
	check &= (newPos.Dist(playerPos, newPos) > _BLOCK_SIZE_ /1.5f);
	if (check) {
		check = check && fullScene[x][y][z] == nullptr;															//check address
		if (check == false) {
			AFigure* coordOwner = ((ACubeActor*)(fullScene[x][y][z]))->owner;
			check = (coordOwner->GetId() == owner->GetId());												//
			
		}
	}
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