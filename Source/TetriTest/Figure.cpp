// Fill out your copyright notice in the Description page of Project Settings.

#include "Figure.h"
#include <vector>
#include "TetriTestGameMode.h"
#include "CubeComponent.h"

Figure::Figure()
{
	blocks.clear();
	figureId = ATetriTestGameMode::GetGameMode()->GetNextId();
}

Figure::~Figure()
{
}

void Figure::AddBlock(AActor* block, long id) {
	blocks[id] = block;
}

void Figure::DestroyBlock(long id, bool destroing) {
	AActor* block = blocks[id];
	ATetriTestGameMode::GetGameMode()->ClearBlockLocation(block);
	blocks.erase(id);
	block->Destroy();
	if (blocks.size() == 0 && !destroing)
		DestroyFigure();
}

void Figure::DestroyFigure() {
	for (auto block : blocks) {
		DestroyBlock(block.first, true);
	}
	delete this;
}

void Figure::Pull(int side) {
	Push(-side);
}

void Figure::Push(int side) {
	FVector pos(0.f, 0.f, 0.f);
	
	std::vector<FVector> figurePos;
	figurePos.clear();
	figurePos.reserve(blocks.size());

	switch (side) {
	case 1:
	case -1: pos.X -= side * _BLOCK_SIZE_; break;
	case 2: pos.Y -= _BLOCK_SIZE_; break;
	case -2: pos.Y += _BLOCK_SIZE_; break;
	case 3: pos.Z -= _BLOCK_SIZE_; break;
	case -3: pos.Z += _BLOCK_SIZE_; break;
	default: break;
	}

	for (auto blockPair : blocks)
		figurePos.push_back(blockPair.second->GetActorLocation() + pos);

	if (ATetriTestGameMode::GetGameMode()->CheckMoveFigure(figurePos, this))
		for (auto blockPair : blocks) {
			AActor* block = blockPair.second;
			ATetriTestGameMode::GetGameMode()->MoveBlockInScene(block, block->GetActorLocation() + pos);
		}
	else {
		if(side == 3)
			ATetriTestGameMode::GetGameMode()->DropFigure();
	}
}

void Figure::RotateX(float mul, FVector& pos) {
	float x = pos.X;
	float y = pos.Z * mul;
	float z = pos.Y * -mul;
	FVector tmp(x, y, z);
	pos = tmp;
}

void Figure::RotateY(float mul, FVector& pos) {
	float x = pos.Z * -mul;
	float y = pos.Y;
	float z = pos.X * mul;
	FVector tmp(x, y, z);
	pos = tmp;
}

void Figure::RotateZ(float mul, FVector& pos) {
	float x = pos.Y * mul;
	float y = pos.X * -mul;
	float z = pos.Z;
	FVector tmp(x, y, z);
	pos = tmp;
}

void Figure::Rotate(int side, FVector blockPos) {
	//calc the rotation axis coordinates
	FVector pos = blockPos;//(0,0,0);

	std::vector<FVector> figurePos;
	figurePos.clear();
	figurePos.reserve(blocks.size());

	//for (auto block : blocks)
	//	pos += block.second->GetActorLocation();
	//pos /= blocks.size();


	for (auto block : blocks) {
		FVector blockPos = block.second->GetActorLocation() - pos; //the position, depended on the middle of the figure
		switch (side) {
		case 1: //x
			RotateX(-1, blockPos);  
			break;
		case -1:  //-x
			RotateX(1, blockPos);
			break;
		case 2:   //y
			RotateY(-1, blockPos);
			break;
		case -2:  //-y
			RotateY(1, blockPos);
			break;
		case 3:   //z
			RotateZ(-1, blockPos);
			break;
		case -3:  //-z
			RotateZ(1, blockPos);
			break;
		}
		figurePos.push_back(blockPos + pos);
	}

	if (ATetriTestGameMode::GetGameMode()->CheckMoveFigure(figurePos, this)) {

		auto blockIter = blocks.begin();

		for (auto blockPos : figurePos) {
			ATetriTestGameMode::GetGameMode()->MoveBlockInScene(blockIter->second, blockPos);
			blockIter++;
		}
	}
}

void Figure::CouterRotate(int side, FVector blockPos) {
	Rotate(-side, blockPos);
}

long Figure::GetId(){ return figureId; }

