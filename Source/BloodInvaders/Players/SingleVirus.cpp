// Fill out your copyright notice in the Description page of Project Settings.

#include "SingleVirus.h"
#include "Engine/CollisionProfile.h"
#include "Virus.h"
#include "Classes/Components/StaticMeshComponent.h"


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
			// Get a refernce from the SingleVirus to the VirusPlayer
			FString parentComponentName = GetParentActor()->GetName();
			AActor* ParentActor = GetParentActor();
			if (ParentActor)
			{
				AVirus* VirusPlayer = Cast<AVirus>(ParentActor);
				if (VirusPlayer)
				{
					// Damage the player
					VirusPlayer->DamagePlayerWithReference(1, VirusPlayer->GetPlayerHealthReference());
				}
			}

			// If we collide with a neutrophil, destroy it
			if (OtherActor->Tags.Contains("Neutrophil"))
			{
				OtherActor->Destroy();
			}

			// Destroy the virus (handle other collisions before this!
			Destroy();
		}
	}
}


UStaticMeshComponent* ASingleVirus::GetVirusMeshComponent() const
{
	return VirusMesh;
}