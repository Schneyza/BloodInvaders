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

	/* The mesh component */
	UPROPERTY(Category = Mesh, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PlayerMeshComponent;

public:
	/* Constructor that sets default values*/
	AVirus();

	/** Returns PlayerMeshComponent subobject **/
	FORCEINLINE class UStaticMeshComponent* GetPlayerMeshComponent() const { return PlayerMeshComponent; }

	virtual void Tick(float DeltaSeconds) override;

	virtual void Move(float DeltaSeconds) override;
	



};
