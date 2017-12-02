// Fill out your copyright notice in the Description page of Project Settings.

#include "Virus.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/CollisionProfile.h"

AVirus::AVirus()
{
	// Create the Sphere Component
	PlayerSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerSphere"));
	RootComponent = PlayerSphereComponent;
	PlayerSphereComponent->SetSphereRadius(1.f);
	PlayerSphereComponent->SetHiddenInGame(true);
	PlayerSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AVirus::BeginPlay()
{
	Super::BeginPlay();
	// Enable Generating hit events on collision
	//PlayerMeshComponent->SetNotifyRigidBodyCollision(true);
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



