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
	UPROPERTY(Category = Mesh, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* PlayerMeshComponent;

public:
	/* Constructor that sets default values*/
	ABacterium();

	/** Returns PlayerMeshComponent subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetPlayerMeshComponent() const { return PlayerMeshComponent; }

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	virtual void Move(float DeltaSeconds) override;
};
