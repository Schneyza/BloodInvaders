// Fill out your copyright notice in the Description page of Project Settings.

#include "Bacterium.h"

#include "Components/SkeletalMeshComponent.h"
#include "Engine/CollisionProfile.h"

ABacterium::ABacterium()
{
	// Create the mesh component
	PlayerMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerMesh"));
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

/* Moves the character. Use this function to specify custom movement behaviour for the virus*/
void ABacterium::Move(float DeltaSeconds)
{
	Super::Move(DeltaSeconds);
}


