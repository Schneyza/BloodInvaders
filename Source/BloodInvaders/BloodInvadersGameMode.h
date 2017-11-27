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

protected:
	/* The player blueprint*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		TSubclassOf<class ABloodInvadersPlayer> PlayerBP;
};



