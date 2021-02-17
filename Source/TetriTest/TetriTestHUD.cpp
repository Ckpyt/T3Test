// Copyright Epic Games, Inc. All Rights Reserved.

#include "TetriTestHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"
#include "TetriTestCharacter.h"

#include <stdlib.h>
#include <stdio.h>

ATetriTestHUD::ATetriTestHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
	CrosshairTex = CrosshairTexObj.Object;
	static ConstructorHelpers::FObjectFinder<UTexture2D> PushTexObj(TEXT("/Game/FirstPerson/Textures/push_icon"));
	PushTex = PushTexObj.Object;
	static ConstructorHelpers::FObjectFinder<UTexture2D> RotateTexObj(TEXT("/Game/FirstPerson/Textures/rotate"));
	RotateTex = RotateTexObj.Object;
	static ConstructorHelpers::FObjectFinder<UTexture2D> ExplTexObj(TEXT("/Game/FirstPerson/Textures/Explosion"));
	BulletTex = ExplTexObj.Object;
	ConstructorHelpers::FObjectFinder<UFont> myFont{ TEXT("Font'/Game/Fonts/arial_Font'") };
	Font = myFont.Object;
}

void ATetriTestHUD::DrawItem(const UTexture2D *itm, const FVector2D pos) {
	FCanvasTileItem TileItem(pos, itm->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(TileItem);
}

void ATetriTestHUD::DrawHUD()
{
	Super::DrawHUD();

	// Draw very simple crosshair

	// find center of the Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// draw the crosshair
	DrawItem(CrosshairTex, FVector2D((Center.X), (Center.Y + 20.0f)));
	// draw gun icons
	DrawItem(BulletTex, FVector2D((Canvas->ClipX - 100), (Canvas->ClipY - 80)));
	DrawItem(RotateTex, FVector2D((Canvas->ClipX - 200), (Canvas->ClipY - 75)));
	DrawItem(PushTex, FVector2D((Canvas->ClipX - 300), (Canvas->ClipY - 80)));
	
	//draw gun charges
	UObject* obj_ptr = StaticLoadObject(UFont::StaticClass(), NULL,
		TEXT("/Game/Fonts/Arial_Font"));
	UFont* font_ptr = Cast<UFont>(obj_ptr);
	
	DrawText(FString::FromInt(ATetriTestCharacter::pushCharges), FLinearColor::White, (Canvas->ClipX - 240), (Canvas->ClipY - 30), font_ptr);
	DrawText(FString::FromInt(ATetriTestCharacter::rotateCharges), FLinearColor::White, (Canvas->ClipX - 130), (Canvas->ClipY - 30), font_ptr);
	DrawText(FString::FromInt(ATetriTestCharacter::destroyCharges), FLinearColor::White, (Canvas->ClipX - 40), (Canvas->ClipY - 30), font_ptr);

	font_ptr->LegacyFontSize = 20;
	//draw scores
	DrawText(FString(FString("Scores:") + FString::FromInt(ATetriTestCharacter::scores)), FLinearColor::White, (0), (10), font_ptr);
}
