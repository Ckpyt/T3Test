// Copyright Epic Games, Inc. All Rights Reserved.

#include "TetriTestGameMode.h"
#include "TetriTestHUD.h"
#include "TetriTestCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATetriTestGameMode::ATetriTestGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ATetriTestHUD::StaticClass();
}
