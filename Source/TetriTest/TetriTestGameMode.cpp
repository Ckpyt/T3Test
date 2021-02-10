// Copyright Epic Games, Inc. All Rights Reserved.


#include "TetriTestGameMode.h"
#include <iostream>     // std::cout, std::ostream, std::ios
#include <fstream>      // std::filebuf
#include <map>

#include "Figure.h"
#include "TetriTestHUD.h"
#include "TetriTestCharacter.h"
#include "CubeActor.h"
#include "UObject/ConstructorHelpers.h"
#include "TetriTestStateBase.h"
#include "TetriTestPlayerState.h"



const int figuresCount = 5;
const int figuresX = 4;
const int figuresY = 2;

const bool AllFigures[figuresCount][figuresX][figuresY] = {
	{{1,1}, {1,1}, {}, {}},		//square
	{{1}, {1,1}, {1}, {}},		//triangle .|.
	{{1}, {1}, {1,1}, {}},		//seven ..|
	{{1}, {1,1}, {0,1}, {}},	//snake .|'
	{{1}, {1}, {1}, {1}}		//stick
};

ATetriTestGameMode* ATetriTestGameMode::instance = nullptr;

ATetriTestGameMode::ATetriTestGameMode()
	: Super()
{
	instance = this;

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD and GameState class
	HUDClass = ATetriTestHUD::StaticClass();
	GameStateClass = ATetriTestStateBase::StaticClass();
	PlayerStateClass = ATetriTestPlayerState::StaticClass();

}

//stop dropping
ATetriTestGameMode::~ATetriTestGameMode() { 
	auto gameState = dynamic_cast<ATetriTestStateBase*>(GameState);
	if (gameState->canFigureDrop)
		ClearScene();

	gameState->canFigureDrop = false;
	if (fullScene != nullptr)
	{
		for (int x = 0; x < gameState->sceneSize; x++) {
			for (int y = 0; y < gameState->sceneSize; y++)
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

	auto gameState = dynamic_cast<ATetriTestStateBase*>(GameState);
	fullScene = new AActor * **[gameState->sceneSize];

	//clear scene
	for (int x = 0; x < gameState->sceneSize; x++) {
		fullScene[x] = new AActor * *[gameState->sceneSize];
		for (int y = 0; y < gameState->sceneSize; y++) {
			fullScene[x][y] = new AActor * [gameState->sceneHeight];
			for (int z = 0; z < gameState->sceneHeight; z++)
				fullScene[x][y][z] = nullptr;
		}
	}

	DropFigure();
}

void ATetriTestGameMode::ClearScene() {
	auto gameState = dynamic_cast<ATetriTestStateBase*>(GameState);

	for (int x = 0; x < gameState->sceneSize; x++) {
		for (int y = 0; y < gameState->sceneSize; y++) {
			for (int z = 0; z < gameState->sceneHeight; z++) {
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
	auto gameState = dynamic_cast<ATetriTestStateBase*>(GameState);

	if (gameState->canFigureDrop) {
		UCubeComponent* owner = nullptr;
		if (GetWorld() != nullptr && (fallingFigure == nullptr || fallingFigure->IsItFalling() == false)) {
			AFigure* fig = AFigure::SpawnFigure(GetWorld());
			srand(time(NULL));
			int figure = rand() % figuresCount;
			bool first = true;
			for (int x = 0; x < figuresX; x++)
				for (int y = 0; y < figuresY; y++)
					if (AllFigures[figure][x][y]) {
						UCubeComponent::SpawnBlock(x, y, fig, GetNextId(), GetWorld());
					}

			fallingFigure = fig;
		}
	}
}

ATetriTestGameMode* ATetriTestGameMode::GetGameMode() { return instance; }

ATetriTestStateBase* ATetriTestGameMode::GetGameState() { 
	ATetriTestGameMode* mode = GetGameMode();
	if (mode == nullptr) return nullptr;
	ATetriTestStateBase* state = dynamic_cast<ATetriTestStateBase*>(mode->GameState);
	return state;
}

void ATetriTestGameMode::ClearBlockLocation(AActor* block) {
	auto gameState = GetGameState();
	if ( gameState->canFigureDrop) {
		FVector pos = block->GetActorLocation();
		int x, y, z;
		CalcXYZFromPos(pos, x, y, z);
		fullScene[x][y][z] = nullptr;
	}
}

void ATetriTestGameMode::ClearFallingFigure() { fallingFigure = nullptr; }

void ATetriTestGameMode::CalcXYZFromPos(const FVector pos, int& x, int& y, int& z) {
	auto gameState = GetGameState();
	x = (((pos.X - gameState->blockSize / 2.f) / gameState->blockSize) + gameState->sceneSize / 2.f);
	y = (((pos.Y - gameState->blockSize / 2.f) / gameState->blockSize) + gameState->sceneSize / 2.f);
	z = ((pos.Z - gameState->blockSize / 2.f) / gameState->blockSize);
	z -= pos.Z < 500 ? 1 : 0;
}

FVector ATetriTestGameMode::CalcPosFromXYZ(const int x, const int y, const int z) {
	auto gameState = GetGameState();
	FVector pos;
	pos.X = (x - gameState->sceneSize / 2.f) * gameState->blockSize + gameState->blockSize / 2.f;
	pos.Y = (y - gameState->sceneSize / 2.f) * gameState->blockSize + gameState->blockSize / 2.f;
	pos.Z = (z) *gameState->blockSize + gameState->blockSize / 2.f + 100.f;
	return pos;
}

void ATetriTestGameMode::CheckAndDestroyLayers(AFigure* figure) {
	auto gameState = dynamic_cast<ATetriTestStateBase*>(GameState);
	std::map<int, int> zCoord;
	zCoord.clear();
	figure->GetZCoordinates(zCoord);
	int layers = 0;
	bool stop = false;
	auto iter = zCoord.end();
	do{
		iter--;
		//if there is one block or more in that layer
		int z = iter->first; 
		bool check = true;
		for (int x = 0; x < gameState->sceneSize && check; x++)
			for (int y = 0; y < gameState->sceneSize && check; y++)
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
	auto gameState = dynamic_cast<ATetriTestStateBase*>(GameState);
	for (int x = 0; x < gameState->sceneSize; x++)
		for (int y = 0; y < gameState->sceneSize; y++)
			((ACubeActor*)fullScene[x][y][z])->owner->DestroyBlock(((ACubeActor*)fullScene[x][y][z])->CubeComp->id);

	//move all layers on one down
	for (int x = 0; x < gameState->sceneSize; x++)
		for (int y = 0; y < gameState->sceneSize; y++)
			for (int newZ = z + 1; newZ + 1 < gameState->sceneHeight; newZ++)
				if (fullScene[x][y][newZ] != nullptr)
					MoveBlockInScene(fullScene[x][y][newZ], fullScene[x][y][newZ]->GetActorLocation(), CalcPosFromXYZ(x, y, newZ - 1));

	//upper layer should be empty
	for (int x = 0; x < gameState->sceneSize; x++)
		for (int y = 0; y < gameState->sceneSize; y++)
			fullScene[x][y][gameState->sceneHeight - 1] = nullptr;

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
	auto gameState = dynamic_cast<ATetriTestStateBase*>(GameState);
	FVector playerPos = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	int x, y, z;
	CalcXYZFromPos(newPos, x, y, z);
	//check scene coordinates
	bool check = (x >= 0 && x < gameState->sceneSize&& y >= 0 && y < gameState->sceneSize&& z >= 0 && z < gameState->sceneHeight);
	float dist = newPos.Dist(playerPos, newPos);
	check &= (dist > (gameState->blockSize /1.5f));
	if (check) {
		//check address
		check = check && fullScene[x][y][z] == nullptr;															
		if (check == false) {
			AFigure* coordOwner = (dynamic_cast<ACubeActor*>(fullScene[x][y][z]))->owner;
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

long ATetriTestGameMode::GetNextId() { 
	auto mode = GetGameMode();
	if (mode == nullptr) return 0;

	auto gameState = dynamic_cast<ATetriTestStateBase*>(GetGameMode()->GameState);
	return ++gameState->lastID;
}