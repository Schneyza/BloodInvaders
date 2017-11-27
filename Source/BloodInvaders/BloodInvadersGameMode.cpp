// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "BloodInvadersGameMode.h"
#include "BloodInvadersCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABloodInvadersGameMode::ABloodInvadersGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/player_bp"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
