// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <vector>
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CubeComponent.h"
#include "TetriTestStateBase.h"
#include "TetriTestGameMode.generated.h"

//#define SCENE_SIZE 4
//#define SCENE_HEIGHT 20
//#define _MAX_HEIGHT_ 19000.f

UCLASS(minimalapi)
class ATetriTestGameMode : public AGameModeBase
{
	GENERATED_BODY()

	//the scene occupation by blocks;
	AActor**** fullScene;

	//should be only one component in the game
	static ATetriTestGameMode* instance;

	void BeginPlay() override;
	AFigure* fallingFigure = nullptr;

public:


	ATetriTestGameMode();
	~ATetriTestGameMode();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	static ATetriTestGameMode* GetGameMode();

	void ClearScene();
	void DropFigure();

	//If there is any full layers, they will be destroyed
	void CheckAndDestroyLayers(AFigure* figure);

	//check a new location of the block: is it inside in the scene?
	bool CheckMoveBlock(const FVector newPos, AFigure* owner);
	//check a new location of the figure: is it inside in the scene?
	bool CheckMoveFigure(std::vector<FVector> &figure, AFigure* owner);

	//moving block in the game scene
	void MoveBlockInScene(AActor* block, const FVector oldPos, const FVector newPos);

	//delete link into the block from scene
	void ClearBlockLocation(AActor* block);

	void ClearFallingFigure();

	static long GetNextId();

	static void CalcXYZFromPos(const FVector pos, int& x, int& y, int& z);

	static FVector CalcPosFromXYZ(const int x, const int y, const int z);

	static ATetriTestStateBase* GetGameState();

	//correcting position in the scene
	static FVector CorrectPosition(FVector pos, long id);

private:
	void DestroyLayer(int z);

	//check a new location of the block: is it inside in the scene?
	bool CheckMoveBlockWithID(const FVector newPos, long id);
};



