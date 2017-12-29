// Fill out your copyright notice in the Description page of Project Settings.

#include "VirusProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"


void AVirusProjectile::Tick(float DeltaSeconds)
{
	if (isAttached)
		return;

	timeToLive -= DeltaSeconds;
	if (timeToLive <= 0) {
		this->Destroy();
	}
}

/** Function to handle the projectile hitting something */
void AVirusProjectile::HitOther(AActor* other) {
	ProjectileMovement->InitialSpeed = 0;
	ProjectileMovement->MaxSpeed = 0;
	ProjectileMovement->StopMovementImmediately();

	ProjectileMesh->SetSimulatePhysics(false);

	this->AttachToActor(other, FAttachmentTransformRules::KeepWorldTransform);

	if (UPrimitiveComponent* PrimitiveComponent = this->FindComponentByClass<UPrimitiveComponent>())
	{
		PrimitiveComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}

	isAttached = true;
}