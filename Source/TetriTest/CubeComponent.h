// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TetriTestCharacter.h"
#include "GameFramework/Actor.h"
#include "Tools.h"
#include "CubeComponent.generated.h"

#define _BLOCK_SIZE_ 1000.f

class AFigure;

//cube component. Used for moving, rotation and delete cube
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TETRITEST_API UCubeComponent : public UActorComponent
{
	GENERATED_BODY()

		static const float size;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* cube;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

	AFigure* figure;

public:	
	FVector location;
	long id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int currentMode;

	// Sets default values for this actor's properties
	UCubeComponent();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	FORCEINLINE int GetMode() const { return (int)currentMode; };

	void Init(GunMode curMode);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	static UCubeComponent* SpawnBlock(const int x, const int y, AFigure* owner, long id, UWorld* const World);

	void UpdateLocalPosition();
	FVector GetLocation();
	void Destroy();

private:
	int CalcCrossedSide(FVector other);

	void DestroyFigure();
	
	
};