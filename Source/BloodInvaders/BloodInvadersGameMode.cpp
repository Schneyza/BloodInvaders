// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "BloodInvadersGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Classes/GameFramework/PlayerStart.h"
#include "Classes/Engine/World.h"
#include "BloodInvadersPlayer.h"
#include "Classes/GameFramework/PlayerController.h"

ABloodInvadersGameMode::ABloodInvadersGameMode()
{

}

void ABloodInvadersGameMode::BeginPlay()
{
	if (PlayerBP != NULL)
	{
		TArray<AActor*> FoundActors;
		UWorld* const World = GetWorld();
		if (World)
		{
			// Get all playerstarts in the world
			UGameplayStatics::GetAllActorsOfClass(World, APlayerStart::StaticClass(), FoundActors);
			// Loop over all playerstarts
			for (int i = 0; i < FoundActors.Num(); i++)
			{
				// Create the player (not the actor)
				UGameplayStatics::CreatePlayer(World, i, true);

				// Check whether player has a playercontroller
				APlayerController* controller = UGameplayStatics::GetPlayerController(World, i);
				if (controller != nullptr) {

					//Set Spawn params
					FActorSpawnParameters SpawnParams;
					SpawnParams.Owner = this;
					SpawnParams.Instigator = Instigator;

					//Set Spawn location
					FVector SpawnLocation = FoundActors[i]->GetActorTransform().GetLocation();

					//Set Spawn Rotation
					FRotator SpawnRotation = FRotator::ZeroRotator;

					// Spawn the player pawn
					ABloodInvadersPlayer* player = World->SpawnActor<ABloodInvadersPlayer>(PlayerBP, SpawnLocation, SpawnRotation, SpawnParams);

					// Possess the newly spawned player pawn
					controller->UnPossess();
					controller->Possess(player);
				}
			}

		}

	}
}
