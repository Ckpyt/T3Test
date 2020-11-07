// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <vector>
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CubeComponent.h"
#include "TetriTestGameMode.generated.h"

#define SCENE_SIZE 4
#define SCENE_HEIGHT 20

UCLASS(minimalapi)
class ATetriTestGameMode : public AGameModeBase
{
	GENERATED_BODY()

	//the scene occupation by blocks;
	UStaticMeshComponent* fullScene[SCENE_SIZE][SCENE_SIZE][SCENE_HEIGHT];

	//should be only one component in the game
	static ATetriTestGameMode* instance;

	//last block ID
	long lastID = 0;

public:
	ATetriTestGameMode();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);

	void ClearScene();
	void DropFigure();

static ATetriTestGameMode* GetGameMode();


	bool CheckMoveBlock(FVector newPos);
	bool CheckRotateFigure(std::vector<FVector> &figure);

	static long GetNextId();
};



