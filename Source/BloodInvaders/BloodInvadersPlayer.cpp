// Fill out your copyright notice in the Description page of Project Settings.

#include "BloodInvadersPlayer.h"
#include "Projectile.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Classes/Components/StaticMeshComponent.h"
#include "Classes/Components/SkeletalMeshComponent.h"
#include "BloodInvadersGameMode.h"

ABloodInvadersPlayer::ABloodInvadersPlayer()
{
	// Movement
	MoveSpeed = 1000.0f;
	// Weapon
	GunOffset = FVector(65.f, 0.f, 0.f);
	FireRate = 0.1f;
	bCanFire = true;
	bFiring = false;

	//Map Boundaries
	XBoundary = 2750.0f;
	YBoundary = 750.0f;

	// Set Playerhealth to defaultValue
	// PlayerHealth = 3;
}

void ABloodInvadersPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	if (ControllerId == 0)
	{
		MoveForwardBinding = "MoveForward_P1";
		MoveRightBinding = "MoveRight_P1";
		FireBinding = "Fire_P1";
		Ability1Binding = "Ability1_P1";
		Ability2Binding = "Ability2_P1";
	}
	else if (ControllerId == 1)
	{
		MoveForwardBinding = "MoveForward_P2";
		MoveRightBinding = "MoveRight_P2";
		FireBinding = "Fire_P2";
		Ability1Binding = "Ability1_P2";
		Ability2Binding = "Ability2_P2";
	}
	InputComponent->BindAxis(MoveForwardBinding);
	InputComponent->BindAxis(MoveRightBinding);
	InputComponent->BindAction(FireBinding, IE_Pressed, this, &ABloodInvadersPlayer::EnableFiring);
	InputComponent->BindAction(FireBinding, IE_Released, this, &ABloodInvadersPlayer::DisableFiring);
	InputComponent->BindAction(Ability1Binding, IE_Pressed, this, &ABloodInvadersPlayer::Ability1);
	InputComponent->BindAction(Ability2Binding, IE_Pressed, this, &ABloodInvadersPlayer::Ability2);
}

void ABloodInvadersPlayer::Tick(float DeltaSeconds)
{
	// Move the character
	Move(DeltaSeconds);

	// Create fire direction vector
	FireShot();
}

void ABloodInvadersPlayer::Move(float DeltaSeconds)
{
	// Find movement direction
	const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
	const float RightValue = GetInputAxisValue(MoveRightBinding);

	// Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
	const FVector MoveDirection = FVector(RightValue, -ForwardValue, 0.f).GetClampedToMaxSize(1.0f);

	// Calculate  movement
	const FVector Movement = MoveDirection * MoveSpeed * DeltaSeconds;

	// If non-zero size, move this actor
	if (Movement.SizeSquared() > 0.0f)
	{
		// Don't change the actors rotation
		const FRotator Rotation = GetTransform().GetRotation().Rotator();
		FHitResult Hit(1.f);
		RootComponent->MoveComponent(Movement, Rotation, true, &Hit);

		// Enforce map boundaries
		if (GetTransform().GetLocation().X > XBoundary) {
			const FVector validPosition = FVector(XBoundary, GetTransform().GetLocation().Y, GetTransform().GetLocation().Z);
			SetActorLocation(validPosition, false);
		}
		if (GetTransform().GetLocation().X < 0) {
			const FVector validPosition = FVector(0, GetTransform().GetLocation().Y, GetTransform().GetLocation().Z);
			SetActorLocation(validPosition, false);
		}
		if (GetTransform().GetLocation().Y > YBoundary) {
			const FVector validPosition = FVector(GetTransform().GetLocation().X, YBoundary, GetTransform().GetLocation().Z);
			SetActorLocation(validPosition, false);
		}
		if (GetTransform().GetLocation().Y < -YBoundary) {
			const FVector validPosition = FVector(GetTransform().GetLocation().X, -YBoundary, GetTransform().GetLocation().Z);
			SetActorLocation(validPosition, false);
		}

		if (Hit.IsValidBlockingHit())
		{
			const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
			const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
			RootComponent->MoveComponent(Deflection, Rotation, true);
		}
	}
}

void ABloodInvadersPlayer::FireShot()
{
	// If we it's ok to fire again
	if (bCanFire == true)
	{
		if (bFiring) {
			// If we are pressing fire stick in a direction
			const FRotator FireRotation = FRotator(0.0f, 0.0f, .0f);
			// Spawn projectile at an offset from this pawn
			const FVector SpawnLocation = GetActorLocation() + GunOffset;

			UWorld* const World = GetWorld();
			if (World != NULL)
			{
				if (Projectile != NULL)
				{
					// spawn the projectile
					World->SpawnActor<AProjectile>(Projectile, SpawnLocation, FireRotation);
				}
				else {
					UE_LOG(LogClass, Error, TEXT("Projectile in BloodInvadersPlayer.cpp is null. Please make sure to assign the Projectile in the Player Blueprint"));
				}
			}

			bCanFire = false;
			World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &ABloodInvadersPlayer::ShotTimerExpired, FireRate);

			// try and play the sound if specified
			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}
			else {
				UE_LOG(LogClass, Error, TEXT("FireSound in BloodInvadersPlayer.cpp is null. Please make sure to assign the FireSound in the Player Blueprint"));
			}

			bCanFire = false;

		}

	}
}

void ABloodInvadersPlayer::EnableFiring() {
	bFiring = true;
	//UE_LOG(LogClass, Log, TEXT("Player with ControllerId %i started firing"), ControllerId);
}

void ABloodInvadersPlayer::DisableFiring() {
	bFiring = false;
	//UE_LOG(LogClass, Log, TEXT("Player with ControllerId %i stopped firing"), ControllerId);
}

void ABloodInvadersPlayer::ShotTimerExpired()
{
	bCanFire = true;
}

void ABloodInvadersPlayer::SetControllerId(int NewControllerId)
{
	ControllerId = NewControllerId;
}

void ABloodInvadersPlayer::Ability1() {}
void ABloodInvadersPlayer::Ability2() {}
