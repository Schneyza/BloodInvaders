// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "BloodInvadersGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Classes/GameFramework/PlayerStart.h"
#include "Classes/Engine/World.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "BloodInvadersPlayer.h"
#include "Classes/GameFramework/PlayerController.h"
#include "Classes/Camera/CameraActor.h"

const int ABloodInvadersGameMode::MaximumNumberOfPlayers = 2;

ABloodInvadersGameMode::ABloodInvadersGameMode()
{
	// Set default camera parameters
	CameraZValue = 1630.0f;
}

void ABloodInvadersGameMode::BeginPlay()
{
	SpawnPlayers();
	//CreateCamera();
	UWorld* const World = GetWorld();
	if (World)
	{
		World->GetTimerManager().SetTimer(InfectableSpawnTimer, this, &ABloodInvadersGameMode::SpawnInfectableCell, InfectableSpawnInterval);
		World->GetTimerManager().SetTimer(BloodCellSpawnTimer, this, &ABloodInvadersGameMode::SpawnBloodCell, BloodCellSpawnInterval);
		World->GetTimerManager().SetTimer(MacrophageSpawnTimer, this, &ABloodInvadersGameMode::ToggleMacrophageSpawn, MacrophageSpawnDelay);

		TArray<AActor*> SpawnVolumes;

		//Get Spawn Volume
		UGameplayStatics::GetAllActorsWithTag(World, "SpawnVolume", SpawnVolumes);
		if (SpawnVolumes.Num() > 0)
		{
			SpawnVolume = Cast<ASpawnVolume>(SpawnVolumes[0]);
		}
		else
		{
			SpawnVolume = nullptr;
		}
	}

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
					if (UGameplayStatics::GetPlayerController(World, i) == NULL)
					{
						UGameplayStatics::CreatePlayer(World, i, true);
					}
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
		Camera->Destroy();
	}
}

void ABloodInvadersGameMode::PlayerDeath(int ControllerId)
{
	// Set the respective player to be dead
	PlayersAlive[ControllerId] = false;

	//If both players the game is over
	if (!PlayersAlive[0] && !PlayersAlive[1])
	{
		LoseGame();
	}
}

void ABloodInvadersGameMode::LoseGame()
{
	UWorld* const World = GetWorld();
	if (World)
	{
		ToggleLoseScreen();
	}
}
void ABloodInvadersGameMode::WinGame()
{
	UWorld* const World = GetWorld();
	if (World)
	{
		ToggleWinScreen();
	}
}


/* Methods for Handling Enemy/Immune System Spawning */

void ABloodInvadersGameMode::ToggleMacrophageSpawn()
{
	UWorld* const World = GetWorld();
	if (World)
	{
		World->GetTimerManager().SetTimer(MacrophageSpawnTimer, this, &ABloodInvadersGameMode::SpawnMacrophage, MacrophageSpawnInterval);
		World->GetTimerManager().SetTimer(NeutrophilMessengerSpawnTimer, this, &ABloodInvadersGameMode::IncreaseNeutrophilMessengerSpawnChance, NeutrophilMessengerSpawnDelay);
	}

}

void ABloodInvadersGameMode::IncreaseNeutrophilMessengerSpawnChance()
{
	UWorld* const World = GetWorld();
	if (World && NeutrophilMessengerSpawnInterval > 0.f)
	{
		if (NeutrophilMessengerSpawnChance < NeutrophilMessengerMaxSpawnChance)
		{
			NeutrophilMessengerSpawnChance += 0.1f;
			//UE_LOG(LogClass, Log, TEXT("Neutrophil Messengers now spawning with %f probability"), NeutrophilMessengerSpawnChance * 100.f);
			World->GetTimerManager().SetTimer(NeutrophilMessengerSpawnTimer, this, &ABloodInvadersGameMode::IncreaseNeutrophilMessengerSpawnChance, NeutrophilMessengerSpawnChanceIncreaseInterval);
		}
		else {
			World->GetTimerManager().ClearTimer(NeutrophilMessengerSpawnTimer);
		}
	}
}


void ABloodInvadersGameMode::IncreaseNeutrophilSpawnChance()
{
	UWorld* const World = GetWorld();
	if (World)
	{
		if (NeutrophilSpawnChance < NeutrophilMaxSpawnChance)
		{
			if (NeutrophilSpawnChance == 0)
			{
				World->GetTimerManager().SetTimer(NeutrophilSpawnTimer, this, &ABloodInvadersGameMode::SpawnNeutrophil, NeutrophilSpawnInterval);
			}
			NeutrophilSpawnChance += 0.1f;
			//UE_LOG(LogClass, Log, TEXT("Neutrophils now spawning with %f probability"), NeutrophilSpawnChance * 100.f);
			if (NeutrophilSpawnChance >= NeutrophilMaxSpawnChance)
			{
				//Stop Neutrophil Messengers from Spawning
				NeutrophilMessengerSpawnChance = 0.f;
				NeutrophilMessengerSpawnInterval = 0.f;
				if (!DendriticMessengerSpawnStarted)
				{
					DendriticMessengerSpawnStarted = true;
					//Start Spawning Dendritic Messengers or atleast start delay for them to spawn
					World->GetTimerManager().SetTimer(DendriticMessengerSpawnTimer, this, &ABloodInvadersGameMode::IncreaseDendriticMessengerSpawnChance, DendriticMessengerSpawnDelay);
				}
			}
		}
	}
}

void ABloodInvadersGameMode::IncreaseDendriticMessengerSpawnChance()
{
	UWorld* const World = GetWorld();
	if (World && DendriticMessengerSpawnInterval > 0.0f)
	{
		if (DendriticMessengerSpawnChance < DendriticMessengerMaxSpawnChance)
		{
			DendriticMessengerSpawnChance += 0.1f;
			//UE_LOG(LogClass, Log, TEXT("Dendritic Messengers now spawning with %f probability"), DendriticMessengerSpawnChance * 100.f);
			World->GetTimerManager().SetTimer(DendriticMessengerSpawnTimer, this, &ABloodInvadersGameMode::IncreaseDendriticMessengerSpawnChance, DendriticMessengerSpawnChanceIncreaseInterval);
		}
		else
		{
			World->GetTimerManager().ClearTimer(DendriticMessengerSpawnTimer);
		}

	}
}

void ABloodInvadersGameMode::IncreaseBCellSpawnChance()
{
	UWorld* const World = GetWorld();
	if (World)
	{
		if (BCellSpawnChance < BCellMaxSpawnChance)
		{
			if (BCellSpawnChance == 0)
			{
				World->GetTimerManager().SetTimer(BCellSpawnTimer, this, &ABloodInvadersGameMode::SpawnBCell, BCellSpawnIntervall);
			}
			BCellSpawnChance += 0.1;
			//UE_LOG(LogClass, Log, TEXT("BCells now spawning with %f probability"), BCellSpawnChance * 100.f);
			if (BCellSpawnChance > BCellMaxSpawnChance)
			{
				//Stop dendritic messengers from spawning
				DendriticMessengerSpawnChance = 0.0f;
				DendriticMessengerSpawnInterval = 0.0f;

				//Start EndGameTimer
				World->GetTimerManager().SetTimer(EndGameTimer, this, &ABloodInvadersGameMode::StopSpawning, EndGameDuration);
				//UE_LOG(LogClass, Log, TEXT("Started EndGame Timer"));
			}
		}
		else 
		{
			//Stop dendritic messengers from spawning
			DendriticMessengerSpawnChance = 0.0f;
			DendriticMessengerSpawnInterval = 0.0f;

			//Start EndGameTimer
			World->GetTimerManager().SetTimer(EndGameTimer, this, &ABloodInvadersGameMode::StopSpawning, EndGameDuration);
			//UE_LOG(LogClass, Log, TEXT("Started EndGame Timer"));
		}
	}
}

void ABloodInvadersGameMode::IncreaseTHelperSpawnChance()
{
	UWorld* const World = GetWorld();
	if (World)
	{
		if (THelperSpawnChance < THelperMaxSpawnChance)
		{
			if (THelperSpawnChance == 0)
			{
				World->GetTimerManager().SetTimer(THelperSpawnTimer, this, &ABloodInvadersGameMode::SpawnTHelper, THelperSpawnInterval);
			}
			THelperSpawnChance += 0.1f;
			//UE_LOG(LogClass, Log, TEXT("THelpers now spawning with %f probability"), THelperSpawnChance * 100.f);
		}
		else
		{
			//Stop dendritic messengers from spawning
			DendriticMessengerSpawnChance = 0.0f;
			DendriticMessengerSpawnInterval = 0.0f;
		}
	}
}

void ABloodInvadersGameMode::SpawnInfectableCell()
{
	//Try to spawn
	//UE_LOG(LogClass, Log, TEXT("Trying to spawn Infectable Cell"));
	if (FMath::RandRange(0.f, 1.f) < InfectableSpawnChance)
	{
		//UE_LOG(LogClass, Log, TEXT("Successfully spawned Infectable Cell"));
		SpawnInfectableBP();
	}
	//Reset Timer
	UWorld* const World = GetWorld();
	if (World)
	{
		World->GetTimerManager().SetTimer(InfectableSpawnTimer, this, &ABloodInvadersGameMode::SpawnInfectableCell, InfectableSpawnInterval);
	}
}

void ABloodInvadersGameMode::SpawnBloodCell()
{
	//Try to spawn
	//UE_LOG(LogClass, Log, TEXT("Trying to spawn BloodCell"));
	if (FMath::RandRange(0.f, 1.f) < BloodCellSpawnChance)
	{
		//UE_LOG(LogClass, Log, TEXT("Successfully spawned BloodCell"));
		SpawnBloodCellBP();
	}
	//Reset Timer
	UWorld* const World = GetWorld();
	if (World)
	{
		World->GetTimerManager().SetTimer(BloodCellSpawnTimer, this, &ABloodInvadersGameMode::SpawnBloodCell, BloodCellSpawnInterval);
	}
}

void ABloodInvadersGameMode::SpawnMacrophage()
{
	//Try to spawn
	//UE_LOG(LogClass, Log, TEXT("Trying to spawn Macrophage"));
	if (FMath::RandRange(0.f, 1.f) < MacrophageSpawnChance)
	{
		//UE_LOG(LogClass, Log, TEXT("Successfully spawned Macrophage"));
		//Spawn Macrophages and pass parameters for neutrophilmessenger spawning
		SpawnMacrophageBP();
	}
	//Reset Timer
	UWorld* const World = GetWorld();
	if (World)
	{
		World->GetTimerManager().SetTimer(MacrophageSpawnTimer, this, &ABloodInvadersGameMode::SpawnMacrophage, MacrophageSpawnInterval);
	}
}

void ABloodInvadersGameMode::SpawnNeutrophil()
{
	//Try to spawn
	//UE_LOG(LogClass, Log, TEXT("Trying to spawn Neutrophil"));
	if (FMath::RandRange(0.f, 1.f) < NeutrophilSpawnChance)
	{
		//UE_LOG(LogClass, Log, TEXT("Successfully spawned Neutrophil"));
		SpawnNeutrophilBP();
	}
	//ResetTimer
	UWorld* const World = GetWorld();
	if (World)
	{
		World->GetTimerManager().SetTimer(NeutrophilSpawnTimer, this, &ABloodInvadersGameMode::SpawnNeutrophil, NeutrophilSpawnInterval);
	}
}

void ABloodInvadersGameMode::SpawnBCell()
{
	//UE_LOG(LogClass, Log, TEXT("Trying to spawn BCell"));
	if (FMath::RandRange(0.f, 1.f) < BCellSpawnChance)
	{
		//UE_LOG(LogClass, Log, TEXT("Successfully spawned BCell"));
		SpawnBCellBP();
	}
	//Reset Timer
	UWorld* const World = GetWorld();
	if (World)
	{
		World->GetTimerManager().SetTimer(BCellSpawnTimer, this, &ABloodInvadersGameMode::SpawnBCell, BCellSpawnIntervall);
	}
}

void ABloodInvadersGameMode::SpawnTHelper()
{
	//UE_LOG(LogClass, Log, TEXT("Trying to spawn THelper"));
	if (FMath::RandRange(0.f, 1.f) < THelperSpawnChance)
	{
		//UE_LOG(LogClass, Log, TEXT("Successfully spawned THelper"));
		SpawnTHelperBP();
	}
	//Reset Timer
	UWorld* const World = GetWorld();
	if (World)
	{
		World->GetTimerManager().SetTimer(THelperSpawnTimer, this, &ABloodInvadersGameMode::SpawnTHelper, THelperSpawnInterval);
	}

}

void ABloodInvadersGameMode::StopSpawning()
{
	//Set all spawn chances to 0
	MacrophageSpawnChance = 0;
	InfectableSpawnChance = 0;
	BloodCellSpawnChance = 0;
	NeutrophilMessengerMaxSpawnChance = 0;
	NeutrophilSpawnChance = 0;
	DendriticMessengerSpawnChance = 0;
	BCellSpawnChance = 0;
	THelperSpawnChance = 0;

	//Start Postgame Timer to return to the main menu
	UWorld* const World = GetWorld();
	if (World)
	{
		World->GetTimerManager().SetTimer(EndGameTimer, this, &ABloodInvadersGameMode::WinGame, PostGameDuration);
		//UE_LOG(LogClass, Log, TEXT("Started PostGame Timer"));
	}
}

