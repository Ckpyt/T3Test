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
	float blockSize = 1000.f;

public:
	const static int chargesPerBlock;
	const float GetBlockSize();
};
