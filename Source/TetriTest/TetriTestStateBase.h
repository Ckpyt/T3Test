// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TetriTestStateBase.generated.h"

/**
 * 
 */
UCLASS()
class TETRITEST_API ATetriTestStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:
	float blockSize = 1000.f;
	int sceneSize = 4;
	int sceneHeight = 20;
	float maxHeight = blockSize * (sceneHeight - 1);

	const static float maxSceneHeigh;

	long lastID = 0;
	bool canFigureDrop = true;

};
