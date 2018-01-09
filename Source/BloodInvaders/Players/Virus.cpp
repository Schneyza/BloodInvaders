// Fill out your copyright notice in the Description page of Project Settings.

#include "Virus.h"
#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/CollisionProfile.h"
#include "Kismet/GameplayStatics.h"
#include "BloodInvadersGameMode.h"
#include "Engine/World.h"
#include "Runtime/Core/Public/Misc/OutputDeviceDebug.h"


const FName AVirus::InfectBinding("Infect");

AVirus::AVirus()
{

	// Create the Sphere Component
	PlayerSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerSphere"));
	RootComponent = PlayerSphereComponent;
	PlayerSphereComponent->SetSphereRadius(1.f);
	PlayerSphereComponent->SetHiddenInGame(true);
	PlayerSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FireRate = 0.1f;

	InitialVirusNumber = 5;
	VirusHealth = InitialVirusNumber;

	nextSingleVirusShotIndex = 0;
}


void AVirus::OnConstruction(const FTransform & Transform) {
	if (SingleVirus != NULL) {
		UWorld* const World = GetWorld();

		if (World != NULL) {
			for (int i = 0; i < InitialVirusNumber; i++) {
				ASingleVirus* sVirus = World->SpawnActor<ASingleVirus>(SingleVirus, GetActorLocation(), FRotator(0.0f, 0.0f, .0f));
				sVirus->parentVirusSwarm = this;
				virusSwarm.Add(sVirus);
			}
		}
	}
	else {
		UE_LOG(LogClass, Error, TEXT("SingleVirus in Virus.cpp is null. Please make sure to assign the SingleVirus in the Virus Blueprint"));
	}
}

void AVirus::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogClass, Log, TEXT("Overlap: %i"), infectableCells.size());
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		if (OtherActor->Tags.Contains("Infect"))
		{
			infectableCells.push_back(OtherActor);
		}
	}
}

void AVirus::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		if (OtherActor->Tags.Contains("Infect"))
		{
			infectableCells.remove(OtherActor);
		}
	}
}

void AVirus::Ability1()
{
	TryInfect();
}

void AVirus::TryInfect() {
	UE_LOG(LogClass, Log, TEXT("Player with ControllerId %i started infecting"), ControllerId);
	UE_LOG(LogClass, Log, TEXT("list: %i"), infectableCells.size());

	if (infectableCells.size() > 0) {
		AActor* actor = infectableCells.front();
		//this->AttachRootComponentToActor(actor);
		this->AttachToActor(actor, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
		this->SetActorRelativeLocation(FVector(0, 0, 0));
	}
}

void AVirus::BeginPlay()
{
	Super::BeginPlay();
}

void AVirus::Tick(float DeltaSeconds)
{
	/* Move the character*/
	Move(DeltaSeconds);

	/* Move the viruses*/
	MoveViruses();

	/* Try to fire a shot*/
	FireShot();
}

/* Moves the character. Use this function to specify custom movement behaviour for the virus*/
void AVirus::Move(float DeltaSeconds)
{
	if (infectedCell) {
		SetActorLocation(infectedCellMesh->GetComponentLocation(), false);
	}
	else {
		Super::Move(DeltaSeconds);
	}
}

void AVirus::FireShot()
{
	if (infectedCell) return;

	// If we it's ok to fire again
	if (bCanFire == true)
	{
		if (bFiring) {
			// If we are pressing fire stick in a direction
			const FRotator FireRotation = FRotator(0.0f, 0.0f, .0f);
			// Spawn projectile at an offset from this pawn


			nextSingleVirusShotIndex = (nextSingleVirusShotIndex + 1) % virusSwarm.Num();
			FVector SpawnLocation = virusSwarm[nextSingleVirusShotIndex]->GetActorLocation() + GunOffset;
			SpawnLocation = FVector(SpawnLocation.X, SpawnLocation.Y, 0);						// Remove Z Component in order to shot on the game plane

			UWorld* const World = GetWorld();
			if (World != NULL)
			{
				if (Projectile != NULL)
				{
					// spawn the projectile
					World->SpawnActor<AProjectile>(Projectile, SpawnLocation, FireRotation);
				}
				else {
					UE_LOG(LogClass, Error, TEXT("Projectile in BloodInvadersPlayer.cpp is null. Please make sure to assign the Projectile in the Player Blueprint"));
				}
			}
			
			bCanFire = false;
			World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &ABloodInvadersPlayer::ShotTimerExpired, FireRate);

			// try and play the sound if specified
			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}
			else {
				UE_LOG(LogClass, Error, TEXT("FireSound in BloodInvadersPlayer.cpp is null. Please make sure to assign the FireSound in the Player Blueprint"));
			}

			bCanFire = false;
			
		}

	}
	
}

void AVirus::SingleVirusGotHit(ASingleVirus* singleVirus)
{
	int amount = 1;

	virusSwarm.Remove(singleVirus);
	singleVirus->Destroy();

	//if we get more or equal damage to our health, the player dies
	if (VirusHealth <= amount)
	{
		VirusHealth = 0;
		bAlive = false;
		// block player input
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, ControllerId);
		if (PlayerController)
		{
			PlayerController->SetCinematicMode(true, false, false, true, true);
		}
		// disable the player character
		APawn* MyCharacter = UGameplayStatics::GetPlayerPawn(this, ControllerId);
		if (MyCharacter)
		{
			MyCharacter->SetActorHiddenInGame(true);
			MyCharacter->SetActorEnableCollision(ECollisionEnabled::NoCollision);
			MyCharacter->SetActorTickEnabled(false);
		}
		// notify the GameMode about the player's death
		UWorld* const World = GetWorld();
		if (World)
		{
			AGameModeBase* GameMode = UGameplayStatics::GetGameMode(World);
			if (GameMode)
			{
				ABloodInvadersGameMode* BIGameMode = Cast<ABloodInvadersGameMode>(GameMode);
				if (BIGameMode)
				{
					BIGameMode->PlayerDeath(ControllerId);
				}
			}
		}
	}
	//otherwise reduce the players health by the specified amount and continue
	else {
		VirusHealth -= amount;
	}
}



bool AVirus::infect(AActor* cell, UStaticMeshComponent* object)
{
	if (infectedCell) {
		return false;
	}
	infectedCell = cell;
	infectedCellMesh = object;

	UWorld* const World = GetWorld();
	World->GetTimerManager().SetTimer(TimerHandle_InfectEnd, this, &AVirus::endInfectEnd, 2);

	return true;
}


void AVirus::endInfectEnd()
{
	FOutputDeviceDebug debug;
	infectedCell->CallFunctionByNameWithArguments(TEXT("EndInfect"), debug, this, true);
	infectedCell = nullptr;
	infectedCellMesh = nullptr;

	// Create new virus
	UWorld* const World = GetWorld();
	ASingleVirus* sVirus = World->SpawnActor<ASingleVirus>(SingleVirus, GetActorLocation(), FRotator(0.0f, 0.0f, .0f));
	sVirus->parentVirusSwarm = this;
	virusSwarm.Add(sVirus);

	//Increase Virus Health
	VirusHealth += 1;

	// Set Z to 0
	FVector loc = GetActorLocation();
	loc.Z = 0;
	SetActorLocation(loc);
}