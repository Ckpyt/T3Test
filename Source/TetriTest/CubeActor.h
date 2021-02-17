// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "CubeActor.generated.h"



UCLASS()
class TETRITEST_API ACubeActor : public AActor
{
	GENERATED_BODY()

public:	
	UPROPERTY(VisibleDefaultsOnly)
	class UStaticMeshComponent* BoxComp;
	UPROPERTY(VisibleDefaultsOnly)
	class UCubeComponent* CubeComp;
	//whole figure owner
	class AFigure* owner;
	
	// Sets default values for this actor's properties
	ACubeActor();

	//for falling. Should be deleted, when falling is finished
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

};
