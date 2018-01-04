// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "BacteriumProjectile.h"
#include "Components/StaticMeshComponent.h"




ABacteriumProjectile::ABacteriumProjectile() : AProjectile()
{
	ProjectileMesh->SetWorldRotation(FRotator(FMath::FRandRange(-180.f, 180.f), FMath::FRandRange(-180.f, 180.f), FMath::FRandRange(-180.f, 180.f)));
}