// Fill out your copyright notice in the Description page of Project Settings.

#include "Figure.h"
#include "CubeActor.h"
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
	if(isItDeleted == false)
		DestroyFigure();
}

void Figure::AddBlock(AActor* block, long id) {
	blocks[id] = block;
}

bool Figure::IsItFalling() { return isItFalling; }

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
	if (isItFalling) {
		ATetriTestGameMode::GetGameMode()->ClearFallingFigure();
		ATetriTestGameMode::GetGameMode()->DropFigure();
	}
	isItDeleted = true;
	delete this;
}

void Figure::Pull(int side) {
	Push(-side);
}

void Figure::Tick() {
	
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

	bool falling = isItFalling;

	for (auto blockPair : blocks)
		figurePos.push_back(blockPair.second->GetActorLocation() + pos);

	if (ATetriTestGameMode::GetGameMode()->CheckMoveFigure(figurePos, this))
		for (auto blockPair : blocks) {
			ACubeActor* block = (ACubeActor*)blockPair.second;
			ATetriTestGameMode::GetGameMode()->MoveBlockInScene(block, block->GetActorLocation(), 
				(falling ? block->GetActorLocation()  : ATetriTestGameMode::CorrectPosition(block->GetActorLocation()))
				+ pos);
			block->CubeComp->UpdateLocalPosition();
		}
	else {
		if (side == 3) {
			isItFalling = false;
			ATetriTestGameMode::GetGameMode()->DropFigure();
		}
	}
}

void Figure::StopFalling() { 
	ATetriTestGameMode* mode = ATetriTestGameMode::GetGameMode();
	FVector pos;

	for (auto block : blocks) {
		FVector loc = ((ACubeActor*)block.second)->CubeComp->GetLocation();
		mode->MoveBlockInScene(block.second, loc, ATetriTestGameMode::CorrectPosition(loc));
		if (loc.Z > pos.Z)
			pos = loc;
	}

	if (pos.Z + 1000.f <= _MAX_HEIGHT_) //is there some space for the next figure?
		mode->DropFigure();
	isItFalling = false; 
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
		case 1:   //x
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
			ACubeActor* block = (ACubeActor*)blockIter->second;
			ATetriTestGameMode::GetGameMode()->MoveBlockInScene(block, block->GetActorLocation(), blockPos);
			block->CubeComp->UpdateLocalPosition();
			blockIter++;
		}
	}
}

void Figure::CouterRotate(int side, FVector blockPos) {
	Rotate(-side, blockPos);
}

long Figure::GetId(){ return figureId; }

