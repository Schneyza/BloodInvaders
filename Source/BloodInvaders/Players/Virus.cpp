// Fill out your copyright notice in the Description page of Project Settings.

#include "Virus.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/CollisionProfile.h"
#include "Kismet/GameplayStatics.h"
#include "BloodInvadersGameMode.h"
#include "Engine/World.h"


const FName AVirus::InfectBinding("Infect");

AVirus::AVirus()
{
	// Create the Sphere Component
	PlayerSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerSphere"));
	RootComponent = PlayerSphereComponent;
	PlayerSphereComponent->SetSphereRadius(1.f);
	PlayerSphereComponent->SetHiddenInGame(true);
	PlayerSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	InitialVirusNumber = 5;
	VirusHealth = InitialVirusNumber;
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
	Super::FireShot();
}

/* Moves the character. Use this function to specify custom movement behaviour for the virus*/
void AVirus::Move(float DeltaSeconds)
{
	Super::Move(DeltaSeconds);
}

void AVirus::DamagePlayer(int amount)
{
	//if we get more or equal damage to our health, the player dies
	if (VirusHealth <= amount)
	{
		VirusHealth = 0;
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



