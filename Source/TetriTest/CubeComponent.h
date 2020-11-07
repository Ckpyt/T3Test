// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TetriTestCharacter.h"
#include "GameFramework/Actor.h"
#include "Tools.h"
#include "CubeComponent.generated.h"

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

	std::vector<UCubeComponent*> figure;
	UCubeComponent* parent = nullptr;

public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int currentMode;

	// Sets default values for this actor's properties
	UCubeComponent();

public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	FORCEINLINE int GetMode() const { return (int)currentMode; };

	void Init(mode curMode);

	void AddChild(UCubeComponent* child);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	static UCubeComponent* SpawnBlock(const int x, const int y, UCubeComponent* owner, UWorld* const World);

private:
	int CalcCrossedSide(FVector other);

	void Push(int side, bool push = false);
	void Pull(int side);
	void Rotate(int side);
	void CouterRotate(int side);
	void DestroyFigure();
	void Destroy();
};
