// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BloodInvadersPlayer.h"
#include "SpawnVolume.h"
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
		ASpawnVolume* SpawnVolume;

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
		float NeutrophilSpawnInterval = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immune System")
		float NeutrophilSpawnChance = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immune System")
		float NeutrophilMaxSpawnChance = 0.8;
	FTimerHandle NeutrophilSpawnTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immune System")
		float DendriticMessengerSpawnDelay = 15;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immune System")
		float DendriticMessengerSpawnInterval = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immune System")
		float DendriticMessengerSpawnChance = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immune System")
		float DendriticMessengerMaxSpawnChance = 0.8;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immune System")
		float DendriticMessengerSpawnChanceIncreaseInterval = 5;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Immune System")
	bool DendriticMessengerSpawnStarted = false;
	FTimerHandle DendriticMessengerSpawnTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immune System")
		float BCellSpawnIntervall = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immune System")
		float BCellSpawnChance = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immune System")
		float BCellMaxSpawnChance = 0.8;
	FTimerHandle BCellSpawnTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immune System")
		float THelperSpawnInterval = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immune System")
		float THelperSpawnChance = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immune System")
		float THelperMaxSpawnChance = 0.8;
	FTimerHandle THelperSpawnTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immune System")
		float EndGameDuration = 120;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immune System")
		float PostGameDuration = 20;
	FTimerHandle EndGameTimer;
	

	
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
		void SpawnInfectableBP();
	UFUNCTION(BlueprintImplementableEvent, Category = "Spawning")
		void SpawnBloodCellBP();
	UFUNCTION(BlueprintImplementableEvent, Category = "Spawning")
		void SpawnMacrophageBP();
	UFUNCTION(BlueprintImplementableEvent, Category = "Spawning")
		void SpawnNeutrophilBP();
	UFUNCTION(BlueprintImplementableEvent, Category = "Spawning")
		void SpawnBCellBP();
	UFUNCTION(BlueprintImplementableEvent, Category = "Spawning")
		void SpawnTHelperBP();

	UFUNCTION(BlueprintCallable, Category = "Immune System")
		void IncreaseNeutrophilSpawnChance();
	UFUNCTION(BlueprintCallable, Category = "Immune System")
		void IncreaseBCellSpawnChance();
	UFUNCTION(BlueprintCallable, Category = "Immune System")
		void IncreaseTHelperSpawnChance();

protected:

private:
	/* Spawns the players and assigns the PlayerControllers*/
	void SpawnPlayers();

	/* Creates the camera, assigns it and sets boundaries for the game*/
	void CreateCamera();

	void ToggleMacrophageSpawn();

	void IncreaseNeutrophilMessengerSpawnChance();
	void IncreaseDendriticMessengerSpawnChance();

	void SpawnInfectableCell();
	void SpawnBloodCell();
	void SpawnMacrophage();
	void SpawnNeutrophil();
	void SpawnBCell();
	void SpawnTHelper();
	
	void StopSpawning();
};



