// Fill out your copyright notice in the Description page of Project Settings.

#include "Bacterium.h"

#include "Components/SkeletalMeshComponent.h"
#include "Engine/CollisionProfile.h"

ABacterium::ABacterium()
{
	// Create the mesh component
	PlayerMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerMesh"));
	PlayerMeshComponent->OnComponentHit.AddDynamic(this, &ABacterium::OnHit);		// set up a notification for when this component hits something
	RootComponent = PlayerMeshComponent;
	PlayerMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
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
		if (OtherActor->Tags.Contains("RBC"))
		{
			OtherActor->Destroy();
		}
	}
}

void ABacterium::Ability1()
{

}

void ABacterium::EatBloodCell()
{

}



