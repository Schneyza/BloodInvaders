// Fill out your copyright notice in the Description page of Project Settings.

#include "Virus.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"

AVirus::AVirus()
{
	// Create the mesh component
	PlayerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMesh"));
	PlayerMeshComponent->OnComponentHit.AddDynamic(this, &AVirus::OnHit);		// set up a notification for when this component hits something
	RootComponent = PlayerMeshComponent;
	PlayerMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
}

void AVirus::Tick(float DeltaSeconds)
{
	/* Move the character*/
	Move(DeltaSeconds);

	/* Try to fire a shot*/
	Super::FireShot();
}

/* Moves the character. Use this function to specify custom movement behaviour for the virus*/
void AVirus::Move(float DeltaSeconds)
{
	Super::Move(DeltaSeconds);
}



