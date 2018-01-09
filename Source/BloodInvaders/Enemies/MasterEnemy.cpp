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

	bCanPowerUp = true;
}

void AMasterEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)  {

	TArray<AActor*> children;
	GetAttachedActors(children);

	// auto iterate though all the actors you already have in the TArray
	for (AActor* child : children)
	{
		child->Destroy();
	}

	AActor::EndPlay(EndPlayReason);
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
		//If the projectile was shot by an enemy, destroy it
		if (Projectile->IsEnemyProjectile())
		{
			OtherActor->Destroy();
		}
		else //if it was shot by a player apply damage
		{
			int DamageToApply = Projectile->GetDamage();

			Projectile->HitOther(this);

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
}

void AMasterEnemy::Die()
{
	OnDeath();
	Destroy();
}

void AMasterEnemy::PowerUp()
{
	if (bCanPowerUp)
	{
		bCanPowerUp = false;
		UE_LOG(LogClass, Log, TEXT("Called PowerUp Method"));
		PowerUpEvent();
	}
}

