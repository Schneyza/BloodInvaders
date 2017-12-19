// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MasterEnemy.generated.h"

UCLASS()
class BLOODINVADERS_API AMasterEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMasterEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Health Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	int MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	int CurrentHealth;

	//Collision Damage Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	int BacteriumDamage;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Handle Collision with other objects */
	UFUNCTION(BlueprintCallable, Category = Enemy)
	virtual void HandleCollision(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Apply Damage*/
	UFUNCTION(BlueprintCallable, Category = Enemy)
	virtual void ApplyDamage(AActor* OtherActor);

	/** What to do when the enemy dies*/
	UFUNCTION(BlueprintCallable, Category = Enemy)
	virtual void Die();

	UFUNCTION(BlueprintPure, Category = Enemy)
	int GetBacteriumDamage() { return BacteriumDamage; }
	
};
