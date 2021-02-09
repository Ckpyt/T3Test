// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TetriTestPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class TETRITEST_API ATetriTestPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	const static int chargesPerBlock;
};
