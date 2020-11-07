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

	// Sets default values for this actor's properties
	ACubeActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
