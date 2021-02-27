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
	{
		block->Destroy();

		if (destroing == false && blocks.size() == 0)
			DestroyFigure();
	}
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

void AFigure::Pull(EBlockSides side) {
	Push(OtherSide(side));
}

void AFigure::Tick(float deltaTime) {
	Super::Tick(deltaTime);

	if (IsItFalling()) {
		ATetriTestStateBase* state = dynamic_cast<ATetriTestStateBase*>(ATetriTestGameMode::GetGameMode()->GameState);
		float deltaZ = deltaTime * state->fallingSpeed;
		FVector pos(0, 0, deltaZ);
		MoveFigure(EBlockSides::plusZ, pos);
	}
}

void AFigure::MoveFigure(EBlockSides side, FVector& pos) {

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
		if (side == EBlockSides::plusZ && isItFalling) {
			StopFalling();
		}
	}
}

void AFigure::Push(EBlockSides side) {
	auto gameState = ATetriTestGameMode::GetGameState();
	FVector pos(0.f, 0.f, 0.f);

	switch (side) {
	case EBlockSides::plusX: pos.X -= gameState->blockSize; break;
	case EBlockSides::minusX: pos.X += gameState->blockSize; break;
	case EBlockSides::plusY: pos.Y -= gameState->blockSize; break;
	case EBlockSides::minusY: pos.Y += gameState->blockSize; break;
	case EBlockSides::plusZ: pos.Z -= gameState->blockSize; break;
	case EBlockSides::minusZ: pos.Z += gameState->blockSize; break;
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
	auto state = ATetriTestGameMode::GetGameState();
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
			blockPos.Z += state->blockSize;
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

	if (pos.Z + state->blockSize <= state->maxHeight) //is there some space for the next figure?
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

void AFigure::Rotate(EBlockSides side, FVector pos) {

	//calc the rotation axis coordinates
	std::vector<FVector> figurePos;
	figurePos.clear();
	figurePos.reserve(blocks.size());


	for (auto block : blocks) {
		//the position, depended on the middle of the figure
		FVector blockPos = block.second->GetActorLocation() - pos; 
		switch (side) {
		case EBlockSides::plusX:
			RotateX(-1, blockPos);  
			break;
		case EBlockSides::minusX:  
			RotateX(1, blockPos);
			break;
		case EBlockSides::plusY:   
			RotateY(-1, blockPos);
			break;
		case EBlockSides::minusY:
			RotateY(1, blockPos);
			break;
		case EBlockSides::plusZ:   
			RotateZ(-1, blockPos);
			break;
		case EBlockSides::minusZ:
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

void AFigure::CouterRotate(EBlockSides side, FVector blockPos) {
	Rotate( OtherSide(side), blockPos);
}

long AFigure::GetId(){ return figureId; }

