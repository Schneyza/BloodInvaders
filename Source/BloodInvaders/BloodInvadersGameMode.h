// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BloodInvadersPlayer.h"
#include "BloodInvadersGameMode.generated.h"

UCLASS(minimalapi)
class ABloodInvadersGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABloodInvadersGameMode();

	virtual void BeginPlay() override;

	/* Specifies the maximum number of players that will be spawned if there are enough PlayerStart objects in the map*/
	static const int MaximumNumberOfPlayers;

protected:
	/* The player blueprint*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		TSubclassOf<class ABloodInvadersPlayer> PlayerBP;
};



