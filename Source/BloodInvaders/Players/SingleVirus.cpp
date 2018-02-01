// Fill out your copyright notice in the Description page of Project Settings.

#include "SingleVirus.h"
#include "Engine/CollisionProfile.h"
#include "Virus.h"
#include "Classes/Components/StaticMeshComponent.h"
#include "../Projectile.h"


// Sets default values
ASingleVirus::ASingleVirus()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Create the mesh component
	VirusMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VirusMesh"));
	VirusMesh->OnComponentHit.AddDynamic(this, &ASingleVirus::OnHit);		// set up a notification for when this component hits something
	RootComponent = VirusMesh;
	VirusMesh->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	PushBackStrength = 2000.f;
}

// Called when the game starts or when spawned
void ASingleVirus::BeginPlay()
{
	Super::BeginPlay();
	// Enable generating hit events
	VirusMesh->SetNotifyRigidBodyCollision(true);
	// Physics
	VirusMesh->SetSimulatePhysics(true);

}

// Called every frame
void ASingleVirus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveWithPlayer();
	MoveWithFluid();

}

void ASingleVirus::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != NULL && OtherActor != this && OtherComp != NULL)
	{
		// if we collide with an enemy
		if (OtherActor->Tags.Contains("Enemy"))
		{
			// Handle Collision with different kinds of enemies
			if (OtherActor->Tags.Contains("Neutrophil"))
			{
				OtherActor->Destroy();
			}
			else if (OtherActor->Tags.Contains("Macrophage"))
			{
				if (!OtherComp->ComponentHasTag("Tentacle"))
				{
					OtherComp->AddForce(NormalImpulse * PushBackStrength);
				}
			}

			if (parentVirusSwarm)
			{
				AVirus* VirusPlayer = Cast<AVirus>(parentVirusSwarm);
				if (VirusPlayer)
				{
					if (!VirusPlayer->IsInvincible())
					{
						// Damage the player
						VirusPlayer->SingleVirusGotHit(this);
					}
				}
			}
		}

		//Handle collision with projectiles
		AProjectile* Projectile = Cast<AProjectile>(OtherActor);
		if (Projectile != nullptr)
		{
			//If the projectile was shot by an enemy, destroy the single virus
			if (Projectile->IsEnemyProjectile())
			{
				AVirus* VirusPlayer = Cast<AVirus>(parentVirusSwarm);
				if (VirusPlayer)
				{
					if (!VirusPlayer->IsInvincible())
					{
						// Damage the player
						VirusPlayer->SingleVirusGotHit(this);
					}
				}
			}
			OtherActor->Destroy();
		}
	}
}


UStaticMeshComponent* ASingleVirus::GetVirusMeshComponent() const
{
	return VirusMesh;
}
