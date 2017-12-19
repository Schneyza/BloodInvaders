// Fill out your copyright notice in the Description page of Project Settings.

#include "MasterProjectile.h"


// Sets default values
AMasterProjectile::AMasterProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMasterProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMasterProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

