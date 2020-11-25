// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <map>

/**
 * Holder of all blocks of the falling figure.
 * Could be only one in the level.
 */
class TETRITEST_API Figure
{
	//all the blocks
	std::map<long, AActor*> blocks;
	
	//rotation of whole figure. degrees = rot*90
	char rotX, rotY, rotZ;

	long figureId = 0;
public:
	Figure();
	~Figure();

	void AddBlock(AActor* block, long id);
	void Push(int side);
	void Pull(int side);
	void Rotate(int side, FVector blockPos);
	void CouterRotate(int side, FVector blockPos);
	void DestroyFigure();

	void DestroyBlock(long id, bool destroying = false);

	long GetId();
private:
	void RotateX(float mul, FVector& pos);
	void RotateY(float mul, FVector& pos);
	void RotateZ(float mul, FVector& pos);
};
