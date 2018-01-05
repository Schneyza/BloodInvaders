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

// Member variables
public:
	/* Specifies the maximum number of players that will be spawned if there are enough PlayerStart objects in the map*/
	static const int MaximumNumberOfPlayers;

	/* Z-Value at which the camera is spawned*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float CameraZValue;

	/* Rotation which the camera spawns with*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	FVector CameraRotation;

	//Immune/Enemy System Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immune System")
		float HeartBeatSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immune System")
		float InfectableSpawnInterval = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immune System")
		float InfectableSpawnChance = 0.5f;
	FTimerHandle InfectableSpawnTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immune System")
		float BloodCellSpawnInterval = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immune System")
		float BloodCellSpawnChance = 0.5f;
	FTimerHandle BloodCellSpawnTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immune System")
		float MacrophageSpawnDelay = 15;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immune System")
		float MacrophageSpawnInterval = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immune System")
		float MacrophageSpawnChance = 0.5f;
	FTimerHandle MacrophageSpawnTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immune System")
		float NeutrophilMessengerSpawnDelay = 15.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immune System")
		float NeutrophilMessengerSpawnInterval = 3.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immune System")
		float NeutrophilMessengerSpawnChance = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immune System")
		float NeutrophilMessengerMaxSpawnChance = 0.8f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immune System")
		float NeutrophilMessengerSpawnChanceIncreaseInterval = 5.f;
	FTimerHandle NeutrophilMessengerSpawnTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immune System")
		float NeutrophilSpawnInterval = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immune System")
		float NeutrophilSpawnChance = 0;
	FTimerHandle NeutriphilSpawnTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immune System")
		float DendriticMessengerSpawnDelay = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immune System")
		float DendriticMessengerSpawnInterval = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immune System")
		float DendriticMessengerSpawnChance = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immune System")
		float MessengerDelay = 0;
	FTimerHandle DendriticMessengerSpawnTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immune System")
		float BCellSpawnIntervall = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immune System")
		float BCellSpawnChance = 0;
	FTimerHandle BCellSpawnTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immune System")
		float THelperSpawnInterval = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immune System")
		float THelperSpawnChance = 0;
	FTimerHandle THelperSpawnTimer;
	

	
protected:
	/* The player blueprints*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	TSubclassOf<class ABloodInvadersPlayer> BacteriumBP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	TSubclassOf<class ABloodInvadersPlayer> VirusBP;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	TArray<bool> PlayersAlive;

private:
	

//Methods
public:
	ABloodInvadersGameMode();

	virtual void BeginPlay() override;

	/* Notifies the GameMode that a player died, if both are dead the game is over*/
	void PlayerDeath(int ControllerId);

	/* Determines what happens when both players are dead*/
	void EndGame();

	UFUNCTION(BlueprintImplementableEvent, Category = "Spawning")
		void SpawnMacrophageBP();

protected:

private:
	/* Spawns the players and assigns the PlayerControllers*/
	void SpawnPlayers();

	/* Creates the camera, assigns it and sets boundaries for the game*/
	void CreateCamera();

	void ToggleMacrophageSpawn();

	void IncreaseNeutrophilMessengerSpawnChance();

	void SpawnInfectableCell();
	void SpawnBloodCell();
	void SpawnMacrophage();
	
};



