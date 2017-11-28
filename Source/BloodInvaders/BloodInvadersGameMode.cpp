// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "BloodInvadersGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Classes/GameFramework/PlayerStart.h"
#include "Classes/Engine/World.h"
#include "BloodInvadersPlayer.h"
#include "Classes/GameFramework/PlayerController.h"

const int ABloodInvadersGameMode::MaximumNumberOfPlayers = 2;

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
			//Determine how many players to spawn
			int PlayersToSpawn = FMath::Min(FoundActors.Num(), MaximumNumberOfPlayers);
			for (int i = 0; i < PlayersToSpawn; i++)
			{
				// Create the player (not the actor); skip first player since he has a player be default
				if (i != 0) {
					UGameplayStatics::CreatePlayer(World, i, true);
				}

				// Check whether player has a playercontroller
				APlayerController* controller = UGameplayStatics::GetPlayerController(World, i);
				if (controller != nullptr) {

					//Set Spawn params
					FActorSpawnParameters SpawnParams;
					SpawnParams.Owner = this;
					SpawnParams.Instigator = Instigator;

					//Set Spawn location to location of PlayerSpawn object
					FVector SpawnLocation = FoundActors[i]->GetActorTransform().GetLocation();

					//Set Spawn Rotation to location of PlayerSpawn object
					FRotator SpawnRotation = FoundActors[i]->GetActorTransform().GetRotation().Rotator();

					// Spawn the player pawn
					ABloodInvadersPlayer* player = World->SpawnActor<ABloodInvadersPlayer>(PlayerBP, SpawnLocation, SpawnRotation, SpawnParams);
					player->SetControllerId(i);

					// Possess the newly spawned player pawn
					controller->UnPossess();
					controller->Possess(player);
				}
			}

		}

	}
}
