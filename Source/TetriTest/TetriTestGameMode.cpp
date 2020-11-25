// Copyright Epic Games, Inc. All Rights Reserved.

#include "TetriTestGameMode.h"
#include <iostream>     // std::cout, std::ostream, std::ios
#include <fstream>      // std::filebuf

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

	DropFigure();
}

//does not work ;-(
void ATetriTestGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) {
	Super::InitGame(MapName, Options, ErrorMessage);
	instance = this;
}

void ATetriTestGameMode::BeginPlay() {
	Super::BeginPlay();
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
	Figure* fig = new Figure();
	srand(time(NULL));
	int figure = rand() % 5;
	bool first = true;
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 2; y++)
			if (AllFigures[figure][x][y]) {
				UCubeComponent::SpawnBlock(x, y, fig, GetNextId(), GetWorld());
			}
}

ATetriTestGameMode* ATetriTestGameMode::GetGameMode() { return instance; }

void ATetriTestGameMode::ClearBlockLocation(AActor* block) {
	FVector pos = block->GetActorLocation();
	int x, y, z;
	CalcXYZFromPos(pos, x, y, z);
	fullScene[x][y][z] = nullptr;
}

void ATetriTestGameMode::CalcXYZFromPos(const FVector pos, int& x, int& y, int& z) {
	x = (((pos.X - _BLOCK_SIZE_ / 2.f) / _BLOCK_SIZE_) + SCENE_SIZE / 2.f);
	y = (((pos.Y - _BLOCK_SIZE_ / 2.f) / _BLOCK_SIZE_) + SCENE_SIZE / 2.f);
	z = ((pos.Z - _BLOCK_SIZE_ / 2.f) / _BLOCK_SIZE_);
}

//Is it a free space on a new locations?
bool ATetriTestGameMode::CheckMoveBlock(const FVector newPos, Figure* owner) {
	FVector playerPos = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	int x, y, z;
	CalcXYZFromPos(newPos, x, y, z);
	bool check = (x >= 0 && x < SCENE_SIZE&& y >= 0 && y < SCENE_SIZE&& z >= 0 && z < SCENE_HEIGHT);			//check scene coordinates
	check &= (newPos.Dist(playerPos, newPos) > _BLOCK_SIZE_ /1.5f);
	if (check) {
		check = check && fullScene[x][y][z] == nullptr;															//check address
		if (check == false) {
			Figure* coordOwner = ((ACubeActor*)(fullScene[x][y][z]))->owner;
			check = (coordOwner->GetId() == owner->GetId());												//
			
		}
	}
	return check;
}

void ATetriTestGameMode::MoveBlockInScene(AActor* block, const FVector newPos) {
	FVector pos = block->GetActorLocation();
	int x, y, z;
	CalcXYZFromPos(pos, x, y, z);
	fullScene[x][y][z] = nullptr;
	CalcXYZFromPos(newPos, x, y, z);
	fullScene[x][y][z] = block;
	block->SetActorLocation(newPos);
}

bool ATetriTestGameMode::CheckMoveFigure(std::vector<FVector> &figurePos, Figure* owner) {
	bool check = true;
	for (FVector pos : figurePos)
		check &= CheckMoveBlock(pos, owner);

	//UE_LOG(LogTemp, Warning, TEXT("check: %d"), check ? 1 : 0);
	return check;
}

long ATetriTestGameMode::GetNextId() { return ++instance->lastID; }