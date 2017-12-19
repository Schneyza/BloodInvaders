// Fill out your copyright notice in the Description page of Project Settings.

#include "MasterEnemy.h"
#include "../Projectiles/MasterProjectile.h"


// Sets default values
AMasterEnemy::AMasterEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMasterEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMasterEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMasterEnemy::OnHit(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	// ApplyDamage to enemy if hit by a Projectile
	AMasterProjectile* Projectile = Cast<AMasterProjectile>(OtherActor);
	if (Projectile != nullptr)
	{
		int DamageToApply = Projectile->GetDamage();

		Projectile->Destroy();

		if (DamageToApply > CurrentHealth)
		{
			//Enemy dies
			Destroy();
		}
		else 
		{
			//Reduce Enemy's Health
			CurrentHealth -= DamageToApply;
		}
	}


	
}

