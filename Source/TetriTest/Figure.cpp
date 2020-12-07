// Fill out your copyright notice in the Description page of Project Settings.

#include "Figure.h"
#include "CubeActor.h"
#include <vector>
#include "TetriTestGameMode.h"
#include "CubeComponent.h"

AFigure::AFigure():
	isItFalling(true)
{
	PrimaryActorTick.bCanEverTick = true;
	blocks.clear();
	figureId = ATetriTestGameMode::GetGameMode()->GetNextId();
	isItFalling = true;
}

AFigure::~AFigure()
{
	if(isItDeleted == false)
		DestroyFigure();
}

AFigure* AFigure::SpawnFigure(UWorld* const World) {
	FRotator SpawnRotation(0.f, 0.f, 0.f);
	FVector SpawnLocation(0,0,0);
	FActorSpawnParameters ActorSpawnParams;

	//Set Spawn Collision Handling Override
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform SpawnTransform(SpawnRotation, SpawnLocation);
	return World->SpawnActor<AFigure>(AFigure::StaticClass(), SpawnLocation, SpawnRotation, ActorSpawnParams); //mode and material in the constructor
}

void AFigure::AddBlock(AActor* block, long id) {
	blocks[id] = block;
}

bool AFigure::IsItFalling() { return isItFalling; }

void AFigure::DestroyBlock(long id, bool destroing) {
	AActor* block = blocks[id];
	ATetriTestGameMode::GetGameMode()->ClearBlockLocation(block);
	blocks.erase(id);
	block->Destroy();
	if (blocks.size() == 0 && !destroing)
		DestroyFigure();
}

void AFigure::DestroyFigure() {
	for (auto block : blocks) {
		DestroyBlock(block.first, true);
	}
	if (isItFalling) {
		ATetriTestGameMode::GetGameMode()->ClearFallingFigure();
		ATetriTestGameMode::GetGameMode()->DropFigure();
	}
	isItDeleted = true;
	Destroy();
}

void AFigure::Pull(int side) {
	Push(-side);
}

void AFigure::Tick(float deltaTime) {
	Super::Tick(deltaTime);

	if (IsItFalling()) {
		float deltaZ = deltaTime * -500; //falling speed, need to move in gameMode
		FVector pos(0, 0, deltaZ);
		MoveFigure(3, pos);
	}
}

void AFigure::MoveFigure(int side, FVector& pos) {

	std::vector<FVector> figurePos;
	figurePos.clear();
	figurePos.reserve(blocks.size());

	for (auto blockPair : blocks)
		figurePos.push_back(blockPair.second->GetActorLocation() + pos);

	if (ATetriTestGameMode::GetGameMode()->CheckMoveFigure(figurePos, this))
		for (auto blockPair : blocks) {
			ACubeActor* block = (ACubeActor*)blockPair.second;
			
			ATetriTestGameMode::GetGameMode()->MoveBlockInScene(block, block->GetActorLocation(),
				(isItFalling ? block->GetActorLocation() : ATetriTestGameMode::CorrectPosition(block->GetActorLocation()))
				+ pos);
			block->CubeComp->UpdateLocalPosition();
		}
	else {
		if (side == 3 && isItFalling) {
			StopFalling();
		}
	}
}

void AFigure::Push(int side) {
	FVector pos(0.f, 0.f, 0.f);

	switch (side) {
	case 1:
	case -1: pos.X -= side * _BLOCK_SIZE_; break;
	case 2: pos.Y -= _BLOCK_SIZE_; break;
	case -2: pos.Y += _BLOCK_SIZE_; break;
	case 3: pos.Z -= _BLOCK_SIZE_; break;
	case -3: pos.Z += _BLOCK_SIZE_; break;
	default: break;
	}

	MoveFigure(side, pos);
	if(isItFalling == false)
		ATetriTestGameMode::GetGameMode()->CheckAndDestroyLayers(this);
}

void AFigure::GetZCoordinates(std::map<int, int>& coord) {
	int x, y, z;
	for (auto blockPair : blocks) {
		ATetriTestGameMode::CalcXYZFromPos(blockPair.second->GetActorLocation(), x, y, z);
		coord[z]++;
	}
}

void AFigure::StopFalling() { 
	isItFalling = false;
	ATetriTestGameMode* mode = ATetriTestGameMode::GetGameMode();
	FVector pos;

	for (auto block : blocks) {
		FVector loc = ((ACubeActor*)block.second)->CubeComp->GetLocation();
		FVector locCorrect = ATetriTestGameMode::CorrectPosition(loc);
		mode->MoveBlockInScene(block.second, loc, locCorrect);
		if (locCorrect.Z > pos.Z)
			pos = locCorrect;
	}

	mode->CheckAndDestroyLayers(this);

	if (pos.Z + 1000.f <= _MAX_HEIGHT_) //is there some space for the next figure?
		mode->DropFigure(); 
}

void AFigure::RotateX(float mul, FVector& pos) {
	float x = pos.X;
	float y = pos.Z * mul;
	float z = pos.Y * -mul;
	FVector tmp(x, y, z);
	pos = tmp;
}

void AFigure::RotateY(float mul, FVector& pos) {
	float x = pos.Z * -mul;
	float y = pos.Y;
	float z = pos.X * mul;
	FVector tmp(x, y, z);
	pos = tmp;
}

void AFigure::RotateZ(float mul, FVector& pos) {
	float x = pos.Y * mul;
	float y = pos.X * -mul;
	float z = pos.Z;
	FVector tmp(x, y, z);
	pos = tmp;
}

void AFigure::Rotate(int side, FVector blockPos) {
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

void AFigure::CouterRotate(int side, FVector blockPos) {
	Rotate(-side, blockPos);
}

long AFigure::GetId(){ return figureId; }

