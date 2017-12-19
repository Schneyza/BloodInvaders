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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Handle Collision with other objects */
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
};
