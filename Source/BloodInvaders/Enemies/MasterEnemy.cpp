// Fill out your copyright notice in the Description page of Project Settings.

#include "MasterEnemy.h"
#include "../Projectile.h"


// Sets default values
AMasterEnemy::AMasterEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MaxHealth = 50;
	CurrentHealth = MaxHealth;

	BacteriumDamage = 20;
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

void AMasterEnemy::HandleCollision(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	ApplyDamage(OtherActor);
}

void AMasterEnemy::ApplyDamage(AActor* OtherActor)
{
	// ApplyDamage to enemy if hit by a Projectile
	AProjectile* Projectile = Cast<AProjectile>(OtherActor);
	if (Projectile != nullptr)
	{
		int DamageToApply = Projectile->GetDamage();

		Projectile->Destroy();

		if (DamageToApply >= CurrentHealth)
		{
			//Enemy dies
			Die();
		}
		else
		{
			//Reduce Enemy's Health
			CurrentHealth -= DamageToApply;
		}
	}
}

void AMasterEnemy::Die()
{
	Destroy();
}

