// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "VirusProjectile.generated.h"

/**
 * 
 */
UCLASS()
class BLOODINVADERS_API AVirusProjectile : public AProjectile
{
	GENERATED_BODY()
	
	
private:
	bool isAttached; 

public:
	virtual void Tick(float DeltaSeconds) override;

	/** Function to handle the projectile hitting something */
	virtual void HitOther(AActor* other) override;
};
