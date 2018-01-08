// Fill out your copyright notice in the Description page of Project Settings.

#include "Bacterium.h"

#include "Components/SkeletalMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "../Enemies/MasterEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "BloodInvadersGameMode.h"
#include "../Projectile.h"

ABacterium::ABacterium()
{
	// Create the mesh component
	//PlayerMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerMesh"));
	//PlayerMeshComponent->OnComponentHit.AddDynamic(this, &ABacterium::OnHit);		// set up a notification for when this component hits something
	//RootComponent = PlayerMeshComponent;
	//PlayerMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	GunOffset = FVector(350.f, 0.f, 0.f);

	BacteriumHealth = 100;
	PushBackStrength = 2000.f;
}

void ABacterium::Tick(float DeltaSeconds)
{
	ABloodInvadersPlayer::Tick(DeltaSeconds);
}

void ABacterium::BeginPlay()
{
	Super::BeginPlay();
	// Enable Generating hit events on collision
	//PlayerMeshComponent->BodyInstance.SetInstanceNotifyRBCollision(true);
	//PlayerMeshComponent->SetNotifyRigidBodyCollision(true);
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
				OtherComp->AddForce(NormalImpulse * PushBackStrength);
			}
			else if (Enemy->ActorHasTag("Protein"))
			{
				Enemy->Destroy();
			}
		}

		//Handle Collision with projectiles
		{
			AProjectile* Projectile = Cast<AProjectile>(OtherActor);
			if (Projectile != nullptr)
			{
				// if the projectile is shot by an enemy, damage the player
				if (Projectile->IsEnemyProjectile())
				{
					DamagePlayer(Projectile->GetDamage());
					OtherActor->Destroy();
				}
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
		BacteriumHealth -= amount;
	}
}

void ABacterium::Ability1()
{

}


void ABacterium::EatBloodCell()
{

}



