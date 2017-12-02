// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BloodInvadersPlayer.h"
#include "Virus.generated.h"

/**
 *
 */
UCLASS()
class BLOODINVADERS_API AVirus : public ABloodInvadersPlayer
{
	GENERATED_BODY()

	/* The Sphere Component that controls the player*/
	UPROPERTY(Category = Player, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* PlayerSphereComponent;

public:
	/* Constructor that sets default values*/
	AVirus();

	/** Returns PlayerMeshComponent subobject **/
	FORCEINLINE class USphereComponent* GetPlayerSphereComponent() const { return PlayerSphereComponent; }

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	virtual void Move(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintImplementableEvent, Category = Player)
	void MoveViruses();


};
