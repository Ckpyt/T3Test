// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <map>
#include "Tools.h"
#include "Figure.generated.h"

/**
 * Holder of all blocks of the figure.
 */
UCLASS()
class TETRITEST_API AFigure : public AActor
{
	GENERATED_BODY()
	//all the blocks in the figure
	std::map<long, AActor*> blocks;
	
	bool isItFalling = true;
	bool isItDeleted = false;

	long figureId = 0;
	
public:
	AFigure();
	~AFigure();

	bool IsItFalling();
	void StopFalling();

	void AddBlock(AActor* block, long id);
	void Push(EBlockSides side);
	void Pull(EBlockSides side);
	void Rotate(EBlockSides side, FVector blockPos);
	void CouterRotate(EBlockSides side, FVector blockPos);

	void DestroyFigure();
	void DestroyBlock(long id, bool destroying = false);

	void GetZCoordinates(std::map<int, int>& coord);
	long GetId();

	virtual void Tick(float DeltaTime) override;

	static AFigure* SpawnFigure(UWorld* const World);
private:
	void RotateX(float mul, FVector& pos);
	void RotateY(float mul, FVector& pos);
	void RotateZ(float mul, FVector& pos);
	void MoveFigure(EBlockSides side, FVector& move);
};
