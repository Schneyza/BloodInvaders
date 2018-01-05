// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BloodInvadersPlayer.h"
#include "Bacterium.generated.h"

/**
 *
 */
UCLASS()
class BLOODINVADERS_API ABacterium : public ABloodInvadersPlayer
{
	GENERATED_BODY()

	/* The mesh component */
	//UPROPERTY(Category = Mesh, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* PlayerMeshComponent;

	/* The Bacterium's Health*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	int BacteriumHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	float PushBackStrength;

public:
	/* Constructor that sets default values*/
	ABacterium();

	/** Returns PlayerMeshComponent subobject **/
	FORCEINLINE class USceneComponent* GetPlayerMeshComponent() const { return RootComponent; }

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	virtual void Move(float DeltaSeconds) override;

	virtual void Ability1() override;

	/* Function to decrease the player's health */
	UFUNCTION(BlueprintCallable, Category = Player)
	void DamagePlayer(int amount);

	void EatBloodCell();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
