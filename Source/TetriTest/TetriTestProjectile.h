// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetriTestCharacter.h"
#include "GameFramework/Actor.h"
#include "Tools.h"
#include "TetriTestProjectile.generated.h"


UCLASS(config=Game)
class ATetriTestProjectile : public AActor
{
	GENERATED_BODY()

	// Sphere collision component 
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	class USphereComponent* CollisionComp;

	// Projectile movement component 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	EGunMode mode;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	ACharacter* player;

	ATetriTestProjectile();
	// used for change a material  
	void Init(EGunMode mode);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// Returns CollisionComp subobject 
	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
	// Returns ProjectileMovement subobject 
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};

