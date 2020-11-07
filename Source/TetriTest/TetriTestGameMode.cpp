// Copyright Epic Games, Inc. All Rights Reserved.

#include "TetriTestGameMode.h"
#include "TetriTestHUD.h"
#include "TetriTestCharacter.h"
#include "CubeComponent.h"
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
	std::cout << "Game mode started!!!" << std::endl;
	ClearScene();
	DropFigure();
}

void ATetriTestGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) {
	Super::InitGame(MapName, Options, ErrorMessage);
	LStream Stream;
	std::cout.rdbuf(&Stream);
	std::cout << "Game mode started!!!" << std::endl;
	
}

void ATetriTestGameMode::ClearScene() {
	for (int z = 0; z < SCENE_HEIGHT; z++) {
		for (int y = 0; y < SCENE_SIZE; y++) {
			for (int x = 0; x < SCENE_SIZE; x++) {
				if (fullScene[x][y][z] != nullptr) {
					UStaticMeshComponent* comp = fullScene[x][y][z];
					fullScene[x][y][z] = nullptr;
					//could have wrong address;
					comp->GetOwner()->Destroy();
				}
			}
		}
	}
}

void ATetriTestGameMode::DropFigure() {
	UCubeComponent* owner = nullptr;
	srand(time(NULL));
	int figure = rand() % 5;
	bool first = true;
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 2; y++)
			if (AllFigures[figure][x][y])
				owner = UCubeComponent::SpawnBlock(x, y, owner, GetWorld());

}

ATetriTestGameMode* ATetriTestGameMode::GetGameMode() { return instance; }

bool ATetriTestGameMode::CheckMoveBlock(FVector newPos) {
	int x = ((newPos.X) / 1000.f + SCENE_SIZE / 2);
	int y = ((newPos.Y) / 1000.f + SCENE_SIZE / 2);
	int z = ((newPos.Z) / 1000.f);
	//UE_LOG(LogTemp, Warning, TEXT("x: %d"), x);
	//UE_LOG(LogTemp, Warning, TEXT("y: %d"), y);
	//UE_LOG(LogTemp, Warning, TEXT("z: %d"), z);
	//UE_LOG(LogTemp, Warning, TEXT("address: %h"), fullScene[z][y][x]);
	//return (x < 0 || x > SCENE_SIZE || y < 0 || y > SCENE_SIZE || z < 0 || z > SCENE_HEIGHT) && //check coordinates
	//	fullScene[z][y][x] != nullptr && !fullScene[z][y][x]->IsPendingKill();					//check coordinates occupation
	return true;
}

bool ATetriTestGameMode::CheckRotateFigure(std::vector<FVector> &figure) {
	bool check = true;
	for (auto pos : figure)
		check &= CheckMoveBlock(pos);
	return check;
}

long ATetriTestGameMode::GetNextId() { return ++instance->lastID; }