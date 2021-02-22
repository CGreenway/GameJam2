// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameJam2GameMode.h"
#include "GameJam2PlayerController.h"
#include "GameJam2Character.h"
#include "UObject/ConstructorHelpers.h"

AGameJam2GameMode::AGameJam2GameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AGameJam2PlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}