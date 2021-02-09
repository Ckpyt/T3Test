// Fill out your copyright notice in the Description page of Project Settings.

#include "Figure.h"
#include "CubeActor.h"
#include <vector>
#include "TetriTestGameMode.h"
#include "TetriTestStateBase.h"
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

	//mode and material in the constructor
	return World->SpawnActor<AFigure>(AFigure::StaticClass(), SpawnLocation, SpawnRotation, ActorSpawnParams); 
}

void AFigure::AddBlock(AActor* block, long id) {
	blocks[id] = block;
}

bool AFigure::IsItFalling() { return isItFalling; }

void AFigure::DestroyBlock(long id, bool destroing) {
	AActor* block = blocks[id];

	ATetriTestGameMode::GetGameMode()->ClearBlockLocation(block);
	blocks.erase(id);

	if (block->IsPendingKillPending() == false) 	
		block->Destroy();
	
	if (destroing == false && blocks.size() == 0 )
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
				(isItFalling ? block->GetActorLocation() : ATetriTestGameMode::CorrectPosition(block->GetActorLocation(), block->CubeComp->id))
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
	auto state = dynamic_cast<ATetriTestStateBase*>(mode->GameState);
	FVector pos;

	std::vector<FVector> figurePos;
	figurePos.clear();
	figurePos.reserve(blocks.size());

	for (auto blockPair : blocks) {
		ACubeActor* cube = dynamic_cast<ACubeActor*>(blockPair.second);
		figurePos.push_back(ATetriTestGameMode::CorrectPosition(cube->GetActorLocation(), cube->CubeComp->id));
	}

	if (mode->CheckMoveFigure(figurePos, this) == false) {
		for (auto &blockPos : figurePos)
			blockPos.Z += _BLOCK_SIZE_;
	}


	auto blockIter = blocks.begin();
	for (int i = 0; i < blocks.size(); i++) {
		auto block = blockIter->second;
		
		auto posCur = figurePos[i];
		FVector loc = ((ACubeActor*)block)->CubeComp->GetLocation();
		mode->MoveBlockInScene(block, loc, posCur);
		if (posCur.Z > pos.Z)
			pos = posCur;

		blockIter++;
	}

	mode->CheckAndDestroyLayers(this);

	if (pos.Z + _BLOCK_SIZE_ <= state->maxHeight) //is there some space for the next figure?
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

void AFigure::Rotate(int side, FVector pos) {

	//calc the rotation axis coordinates
	std::vector<FVector> figurePos;
	figurePos.clear();
	figurePos.reserve(blocks.size());

	//for (auto block : blocks)
	//	pos += block.second->GetActorLocation();
	//pos /= blocks.size();


	for (auto block : blocks) {
		//the position, depended on the middle of the figure
		FVector blockPos = block.second->GetActorLocation() - pos; 
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

