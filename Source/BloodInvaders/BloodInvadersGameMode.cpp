// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "BloodInvadersGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Classes/GameFramework/PlayerStart.h"
#include "Classes/Engine/World.h"
#include "BloodInvadersPlayer.h"
#include "Classes/GameFramework/PlayerController.h"
#include "Classes/Camera/CameraActor.h"

const int ABloodInvadersGameMode::MaximumNumberOfPlayers = 2;

ABloodInvadersGameMode::ABloodInvadersGameMode()
{
	// Set default camera parameters
	CameraZValue = 1630.0f;
	CameraRotation = FVector(0.f, 0.f, -90.f);
}

void ABloodInvadersGameMode::BeginPlay()
{
	SpawnPlayers();
	CreateCamera();
}

void ABloodInvadersGameMode::SpawnPlayers()
{
	//UE_LOG(LogClass, Log, TEXT("Calles SpawnPlayers"));
	UWorld* const World = GetWorld();
	if (World)
	{
		//UE_LOG(LogClass, Log, TEXT("World not null"));
		//Spawn the players and set their controllers
		TArray<AActor*> FoundActors;
		if (BacteriumBP != NULL && VirusBP != NULL)
		{
			//UE_LOG(LogClass, Log, TEXT("PlayerBP not null"));
			// Get all playerstarts in the world
			UGameplayStatics::GetAllActorsOfClass(World, APlayerStart::StaticClass(), FoundActors);
			//Determine how many players to spawn
			int PlayersToSpawn = FMath::Min(FoundActors.Num(), MaximumNumberOfPlayers);
			PlayersAlive = { false, false };
			for (int i = 0; i < PlayersToSpawn; i++)
			{
				if (FoundActors[i]->GetName().Contains("PlayerStartPie"))
				{
					continue;
				}
				// Create the player (not the actor); skip first player since he has a player be default
				if (i != 0) {
					//UE_LOG(LogClass, Log, TEXT("Create Player"));
					UGameplayStatics::CreatePlayer(World, i, true);
				}

				//Set the player to be alive
				PlayersAlive[i] = true;

				// Check whether player has a playercontroller
				APlayerController* controller = UGameplayStatics::GetPlayerController(World, i);
				if (controller != nullptr) {
					//UE_LOG(LogClass, Log, TEXT("Spawn pawn and assign PlayerController"));

					//Set Player Spawn params
					FActorSpawnParameters SpawnParams;
					SpawnParams.Owner = this;
					SpawnParams.Instigator = Instigator;

					//Set Spawn location to location of PlayerSpawn object
					FVector SpawnLocation = FoundActors[i]->GetActorTransform().GetLocation();

					//Set Spawn Rotation to rotation of PlayerSpawn object
					FRotator SpawnRotation = FoundActors[i]->GetActorTransform().GetRotation().Rotator();

					//Spawn the Bacterium for the first player and the virus for the second
					ABloodInvadersPlayer* player;
					if (i == 0)
					{
						player = World->SpawnActor<ABloodInvadersPlayer>(VirusBP, SpawnLocation, SpawnRotation, SpawnParams);
					}
					else
					{
						player = World->SpawnActor<ABloodInvadersPlayer>(BacteriumBP, SpawnLocation, SpawnRotation, SpawnParams);
					}
					player->SetControllerId(i);

					// Possess the newly spawned player pawn
					controller->UnPossess();
					controller->Possess(player);
				}
			}

		}
		else {
			UE_LOG(LogClass, Error, TEXT("PlayerBP in BloodInvadersGameMode.cpp is null! Make sure to assign PlayerBP in the GameMode Blueprint!"));
		}

	}
}

void ABloodInvadersGameMode::CreateCamera()
{
	UWorld* const World = GetWorld();
	if (World)
	{
		//Create the camera, set it active, set boundaries
		//Set Camera Spawn params
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;

		//Set Camera Spawn location
		FVector SpawnLocation = FVector(0.f, 0.f, CameraZValue);

		//Set Camera Spawn Rotation
		FRotator SpawnRotation = CameraRotation.Rotation();

		// Spawn the player pawn
		ACameraActor* Camera = World->SpawnActor<ACameraActor>(ACameraActor::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
		APlayerController* Controller = UGameplayStatics::GetPlayerController(World, 0);
		Controller->SetViewTargetWithBlend(Camera, 0.f, VTBlend_Linear, 0.f, false);
	}
}

void ABloodInvadersGameMode::PlayerDeath(int ControllerId)
{
	// Set the respective player to be dead
	PlayersAlive[ControllerId] = false;

	//If both players the game is over
	if (!PlayersAlive[0] && !PlayersAlive[1])
	{
		EndGame();
	}
}

void ABloodInvadersGameMode::EndGame()
{
	UWorld* const World = GetWorld();
	if (World)
	{
		// Load the MainMenu level
		UGameplayStatics::OpenLevel((UObject*)UGameplayStatics::GetGameInstance(World), FName(TEXT("MainMenu")));
	}
}
