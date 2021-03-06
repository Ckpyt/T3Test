// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Tools.h"
#include "TetriTestCharacter.generated.h"


class UInputComponent;

UCLASS(config=Game)
class ATetriTestCharacter : public ACharacter
{
	GENERATED_BODY()

	EGunMode currentMode;

	// Pawn mesh: 1st person view (arms; seen only by self) 
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	class USkeletalMeshComponent* Mesh1P;

	// Gun mesh: 1st person view (seen only by self) 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Mesh)
	class USkeletalMeshComponent* FP_Gun;

	// Location on gun mesh where projectiles should spawn.
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* FP_MuzzleLocation;

	// Gun mesh: VR view (attached to the VR controller directly, no arm, just the actual gun) 
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* VR_Gun;

	// Location on VR gun mesh where projectiles should spawn. 
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* VR_MuzzleLocation;

	// First person camera 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	// Motion controller (right hand)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* R_MotionController;

	// Motion controller (left hand) 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* L_MotionController;

	// Used for jetPack. 
	float lastJumpTime = 0;
	// maximum jump speed. Used for jetPack 
	float maxJumpVelocity = 0;

public:
	static int pushCharges;
	static int rotateCharges;
	static int destroyCharges;
	static int scores;

	// Base turn rate, in deg/sec. Other scaling may affect final turn rate. 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	// Base look up/down rate, in deg/sec. Other scaling may affect final rate. 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	// Gun muzzle's offset from the characters location 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	// Projectile class to spawn 
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class ATetriTestProjectile> ProjectileClass;

	// Sound to play each time we fire 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	class USoundBase* FireSound;

	// AnimMontage to play each time we fire 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation;

	// Whether to use motion controller location for aiming. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	uint32 bUsingMotionControllers : 1;

	DECLARE_DELEGATE_OneParam(FChooseDelegate, EGunMode);
	DECLARE_DELEGATE_OneParam(FFireDelegate, bool);

	ATetriTestCharacter();

	static void AddPushCharges();
	static void AddRotateCharges();
	static void AddDestroyCharges();

	// Returns Mesh1P subobject 
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	// Returns FirstPersonCameraComponent subobject 
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

protected:

	virtual void BeginPlay();
	// Fires a projectile. 
	void OnFire(bool alternative = false);

	// Resets HMD orientation and position in VR. 
	void OnResetVR();

	// Handles moving forward/backward 
	void MoveForward(float Val);

	// Handles strafing movement, left and right 
	void MoveRight(float Val);

	// Handles jump and jetpack movement 
	void Jump();

	UFUNCTION(BlueprintCallable, Category = SomeCategory)
	void Choose(EGunMode gunMode);

	// Handles stop jumping 
	void StopJumping();

	void JetPack(float value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

#pragma region touch
	struct TouchData
	{
		TouchData() { bIsPressed = false;Location=FVector::ZeroVector;}
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);
	TouchData	TouchItem;
#pragma endregion
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/* 
	 * Configures input for touchscreen devices if there is a valid touch interface for doing so 
	 *
	 * @param	InputComponent	The input component pointer to bind controls to
	 * @returns true if touch controls were enabled.
	 */
	bool EnableTouchscreenMovement(UInputComponent* InputComponent);
};

