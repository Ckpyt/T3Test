// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TetriTestHUD.generated.h"

UCLASS()
class ATetriTestHUD : public AHUD
{
	GENERATED_BODY()

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MOO)
	UFont* Font;

public:
	ATetriTestHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;
	/** Push asset pointer */
	class UTexture2D* PushTex;
	/** Rotate asset pointer */
	class UTexture2D* RotateTex;
	/** Bullet asset pointer */
	class UTexture2D* BulletTex;

	void DrawItem(const UTexture2D* itm, const FVector2D pos);
};

