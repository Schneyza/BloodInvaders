// Fill out your copyright notice in the Description page of Project Settings.

#include "Bacterium.h"

#include "Components/SkeletalMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "../Enemies/MasterEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "BloodInvadersGameMode.h"

ABacterium::ABacterium()
{
	// Create the mesh component
	PlayerMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerMesh"));
	PlayerMeshComponent->OnComponentHit.AddDynamic(this, &ABacterium::OnHit);		// set up a notification for when this component hits something
	RootComponent = PlayerMeshComponent;
	PlayerMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	BacteriumHealth = 100;
}

void ABacterium::Tick(float DeltaSeconds)
{
	/* Move the character*/
	Move(DeltaSeconds);

	/* Try to fire a shot*/
	Super::FireShot();
}

void ABacterium::BeginPlay()
{
	Super::BeginPlay();
	// Enable Generating hit events on collision
	PlayerMeshComponent->BodyInstance.SetInstanceNotifyRBCollision(true);
	PlayerMeshComponent->SetNotifyRigidBodyCollision(true);
}

/* Moves the character. Use this function to specify custom movement behaviour for the virus*/
void ABacterium::Move(float DeltaSeconds)
{
	Super::Move(DeltaSeconds);
}

void ABacterium::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		//Handle Collision with Red Blood Cell
		if (OtherActor->Tags.Contains("RBC"))
		{
			OtherActor->Destroy();
		}
		//Handle Collision with Enemies
		AMasterEnemy* Enemy = Cast<AMasterEnemy>(OtherActor);
		if (Enemy != nullptr)
		{
			int DamageToApply = Enemy->GetBacteriumDamage();
			DamagePlayer(DamageToApply);

			//Handle Collision based on EnemyType
			if (Enemy->ActorHasTag("Neutrophil"))
			{
				Enemy->Destroy();
			}
			else if (Enemy->ActorHasTag("Macrophage"))
			{
				UE_LOG(LogClass, Error, TEXT("Try to Bounce back Macrophage"));
				OtherComp->AddForce(NormalImpulse * 20.f);
			}
		}
	}
}

void ABacterium::DamagePlayer(int amount)
{
	//if we get more or equal damage to our health, the player dies
	if (BacteriumHealth <= amount)
	{
		BacteriumHealth = 0;
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
		BacteriumHealth -= amount;
	}
}

void ABacterium::Ability1()
{

}


void ABacterium::EatBloodCell()
{

}



