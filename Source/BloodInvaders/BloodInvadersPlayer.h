// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BloodInvadersPlayer.generated.h"

UCLASS()
class BLOODINVADERS_API ABloodInvadersPlayer : public APawn
{
	GENERATED_BODY()

//Member Variables
public:
	

	/** Offset from the player's location to spawn projectiles */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	FVector GunOffset;

	/* How fast the weapon will fire */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float FireRate;

	/* The speed our player's moves around the level */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;

	/** Sound to play each time we fire */
	UPROPERTY(Category = Audio, EditAnywhere, BlueprintReadWrite)
	class USoundBase* FireSound;

	// Static names for axis bindings
	static const FName MoveForwardBinding;
	static const FName MoveRightBinding;
	static const FName FireBinding;

	/* Borders restricting the movement of the player to the screen boundaries*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Boundaries, meta = (AllowPrivateAccess = "true"))
	float XBoundary;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Boundaries, meta = (AllowPrivateAccess = "true"))
	float YBoundary;

protected:
	/* The projectile that is spawned when the player shoots*/
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AProjectile> Projectile;

	/* Is the player currently firing?*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	bool bFiring;

	/* The player's controller id*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Controller, meta = (AllowPrivateAccess = "true"))
	int ControllerId;

	/* The player's Health*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	int PlayerHealth;

private:
	/* Flag to control firing  */
	uint32 bCanFire : 1;

	/** Handle for efficient management of ShotTimerExpired timer */
	FTimerHandle TimerHandle_ShotTimerExpired;

// Methods
public:
	// Sets default values for this pawn's properties
	ABloodInvadersPlayer();

	virtual void Tick(float DeltaSeconds) override;

	/* Function to set up Keybindings*/
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	/* Function that moves the player according to user inputs. Called every tick*/
	virtual void Move(float DeltaSecons);

	/** Function to handle the player hitting something */
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/* Function to decrease the player's health */
	UFUNCTION(BlueprintCallable, Category = Player)
	virtual void DamagePlayer(int amount);

	/* Returns the current HealthValue of the player*/
	UFUNCTION(BlueprintPure, Category = Player)
	int GetPayerHealth();

	/* Fire a shot in the specified direction */
	virtual void FireShot();
	void EnableFiring();
	void DisableFiring();
	/* Handler for the fire timer expiry */
	void ShotTimerExpired();

	//Accessor functions for the player's controller id
	void SetControllerId(int NewControllerId);
	FORCEINLINE int GetControllerId() const { return ControllerId; }
	
protected:



private

};