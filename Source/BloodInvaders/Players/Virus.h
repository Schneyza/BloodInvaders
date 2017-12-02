// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <list>
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

	/* Is the player currently infecting?*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	bool bInfecting;

	std::list<AActor*> infectableCells;

public:
	/* Constructor that sets default values*/
	AVirus();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/** Returns PlayerMeshComponent subobject **/
	FORCEINLINE class USphereComponent* GetPlayerSphereComponent() const { return PlayerSphereComponent; }

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	virtual void Move(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintImplementableEvent, Category = Player)
	void MoveViruses();

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	static const FName InfectBinding;
	void TryInfect();
};
