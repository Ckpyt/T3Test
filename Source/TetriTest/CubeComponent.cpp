// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeComponent.h"
#include "CubeActor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "TetriTestProjectile.h"
#include "TetriTestGameMode.h"
#include "TetriTestStateBase.h"
#include "Figure.h"
#include <stdio.h>



// Sets default values
UCubeComponent::UCubeComponent()
{
	TArray<UStaticMeshComponent*> Components;
	auto owner = GetOwner();
	if (owner != nullptr) {
		owner->GetComponents<UStaticMeshComponent>(Components, true);
		if (Components.Num() > 0) {

			cube = Components[0];
			CollisionComp = Components[0];
			CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
			CollisionComp->OnComponentHit.AddDynamic(this, &UCubeComponent::OnHit);		// set up a notification for when this component hits something blocking
			CollisionComp->bApplyImpulseOnDamage = true;
			CollisionComp->bIgnoreRadialForce = true;
			CollisionComp->bIgnoreRadialImpulse = true;
		}
	}
}

int UCubeComponent::CalcCrossedSide(FVector other) {
	FVector pos = cube->GetComponentLocation();
	
	//UE_LOG(LogTemp, Warning, TEXT("Other vector: x:%f, y:%f, z:%f"), other.X, other.Y, other.Z);

	FVector impulse = other - pos;
	//UE_LOG(LogTemp, Warning, TEXT("impulse vector: x:%f, y:%f, z:%f"), impulse.X, impulse.Y, impulse.Z);
	float tmpX = impulse.X > 0 ? impulse.X : -impulse.X;
	float tmpY = impulse.Y > 0 ? impulse.Y : -impulse.Y;

	//UE_LOG(LogTemp, Warning, TEXT("impulse vector: x:%f, y:%f"), X, Y);

	if (tmpX > 520.0f) {
		return impulse.X > 0 ? 1 : -1;
	}
	else if (tmpY > 520.0f) {
		return impulse.Y > 0 ? 2 : -2;
	}
	else {
		return impulse.Z > 0 ? 3 : -3;
	}

	return 0;
}

void UCubeComponent::UpdateLocalPosition() {
	location = GetOwner()->GetActorLocation();
}

FVector UCubeComponent::GetLocation() { return location; }

void UCubeComponent::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherComp != nullptr) /*&& OtherComp->IsSimulatingPhysics()*/)
	{
		//OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		ATetriTestProjectile* bullet = Cast<ATetriTestProjectile>(OtherActor);
		if (bullet != nullptr) {
			int mode = bullet->mode;
			int side = CalcCrossedSide(OtherActor->GetActorLocation());

			switch (mode) {
			case 0: figure->Push(side); break;
			case 3: figure->Pull(side); break;

			case 1: figure->Rotate(side, this->GetOwner()->GetActorLocation()); break;
			case 4: figure->CouterRotate(side, this->GetOwner()->GetActorLocation()); break;

			case 2: Destroy(); break;
			case 5: DestroyFigure(); break;
			}
		}
	}
}

void UCubeComponent::Init(GunMode curMode) {currentMode = (int)curMode;}

void UCubeComponent::DestroyFigure() {
	figure->DestroyFigure();
}

void UCubeComponent::Destroy() {

	switch (currentMode) {
	case 0: ATetriTestCharacter::AddPushCharges(); break;
	case 1: ATetriTestCharacter::AddRotateCharges(); break;
	case 2: ATetriTestCharacter::AddDestroyCharges(); break;
	}
	figure->DestroyBlock(id);
}

UCubeComponent* UCubeComponent::SpawnBlock(const int x, const int y, AFigure* owner, long id, UWorld* const World) {
	ATetriTestGameMode* mode = ATetriTestGameMode::GetGameMode();
	auto gameState = ATetriTestGameMode::GetGameState();
	UCubeComponent* cube = nullptr;
	static const TSubclassOf<class ACubeActor> ProjectileClass;

	if (World != NULL)
	{

		FRotator SpawnRotation(0.f,0.f,0.f);
		FVector SpawnLocation(((float)x - 1.5f) * gameState->blockSize, 
			((float)y - 1.5f) * gameState->blockSize, gameState->maxHeight - gameState->blockSize / 2);
		FActorSpawnParameters ActorSpawnParams;

		//Set Spawn Collision Handling Override
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FTransform SpawnTransform(SpawnRotation, SpawnLocation);

		//mode and material in the constructor
		ACubeActor* actor = World->SpawnActor<ACubeActor>(ACubeActor::StaticClass(), SpawnLocation, SpawnRotation, ActorSpawnParams); 
		if (actor != nullptr) {
			actor->SetOwner(owner);
			actor->owner = owner;
			cube = actor->CubeComp;
			cube->id = id;
			cube->figure = owner;
			owner->AddBlock(actor, id);
		}
	}

	return cube;
}