// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <list>
#include "CoreMinimal.h"
#include "BloodInvadersPlayer.h"
#include "SingleVirus.h"
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

	/* The initial number of SingleViruses, also the inital amount of Health for the virus*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	int InitialVirusNumber;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	int VirusHealth;

	/* The single virus that make up the swarm*/
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ASingleVirus> SingleVirus;

	std::list<AActor*> infectableCells;

private:
	int nextSingleVirusShotIndex = 0;

	AActor* infectedCell = nullptr;
	UStaticMeshComponent* infectedCellMesh = nullptr;
	FTimerHandle TimerHandle_InfectEnd;

	TArray < ASingleVirus * > virusSwarm;

	/*Is the virus currently invincible*/
	bool bInvincible;

	void TurnOffInvincibility();

public:
	/* Constructor that sets default values*/
	AVirus();

	// Creates SingleVirus Swarm
	virtual void OnConstruction(const FTransform & Transform) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/** Returns PlayerMeshComponent subobject **/
	FORCEINLINE class USphereComponent* GetPlayerSphereComponent() const { return PlayerSphereComponent; }

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	virtual void Move(float DeltaSeconds) override;


	virtual void FireShot() override;
	
	UFUNCTION(BlueprintImplementableEvent, Category = Player)
	void MoveViruses();

	virtual void Ability1() override;

	UFUNCTION(BlueprintCallable, Category = Player)
	bool infect(AActor* cell, UStaticMeshComponent* object);
	void endInfectEnd();

	static const FName InfectBinding;
	void TryInfect();
	
	/* Called by ASingleVirus when hit */
	void SingleVirusGotHit(ASingleVirus* singleVirus);

	UFUNCTION(BlueprintPure, Category = Virus)
	bool IsInvincible();
	UFUNCTION(BlueprintCallable, Category = Virus)
	void SetInvincible(bool invincible);

	UFUNCTION(BlueprintIMplementableEvent, Category = Virus)
		void GotHitEvent();

	UFUNCTION(BlueprintCallable, Category = Virus)
		TArray<ASingleVirus*> GetSingleViruses();
	UFUNCTION(BlueprintImplementableEvent, Category = Virus)
		void HealEffect();
};


