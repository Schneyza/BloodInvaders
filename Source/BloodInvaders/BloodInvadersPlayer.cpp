// Fill out your copyright notice in the Description page of Project Settings.

#include "BloodInvadersPlayer.h"
#include "Projectile.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

const FName ABloodInvadersPlayer::MoveForwardBinding("MoveForward");
const FName ABloodInvadersPlayer::MoveRightBinding("MoveRight");
const FName ABloodInvadersPlayer::FireBinding("Fire");

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
	XBoundary = 660.0f;
	YBoundary = 1180.0f;

	// Set Playerhealth to defaultValue
	PlayerHealth = 3;
}

void ABloodInvadersPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	// set up gameplay key bindings
	PlayerInputComponent->BindAxis(MoveForwardBinding);
	PlayerInputComponent->BindAxis(MoveRightBinding);
	PlayerInputComponent->BindAction(FireBinding, IE_Pressed, this, &ABloodInvadersPlayer::EnableFiring);
	PlayerInputComponent->BindAction(FireBinding, IE_Released, this, &ABloodInvadersPlayer::DisableFiring);
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
	const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);

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
		if (GetTransform().GetLocation().X < -XBoundary) {
			const FVector validPosition = FVector(-XBoundary, GetTransform().GetLocation().Y, GetTransform().GetLocation().Z);
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
			const FRotator FireRotation = FRotator(0.0f, 90.0f, .0f);
			// Spawn projectile at an offset from this pawn
			const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

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

void ABloodInvadersPlayer::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		if (OtherActor->Tags.Contains("Enemy"))
		{
			//If we have hit an enemy, damage the player
			DamagePlayer(1);
			if (OtherActor->Tags.Contains("Neutrophil"))
			{
				//If we hit a Neutrophil, destroy the neutrophil
				OtherActor->Destroy();
			}
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

void ABloodInvadersPlayer::DamagePlayer(int amount)
{
	//if we get more or equal damage to our health, the player dies
	if (PlayerHealth <= amount)
	{
		PlayerHealth = 0;
		// kill
		// block player input
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, ControllerId);
		if (PlayerController)
		{
			PlayerController->SetCinematicMode(true, false, false, true, true);
		}
		// ragdoll the character
		APawn* MyCharacter = UGameplayStatics::GetPlayerPawn(this, ControllerId);
		if (MyCharacter)
		{
			MyCharacter->Destroy();
		}
	}
	//otherwise reduce the players health by the specified amount and continue
	else {
		PlayerHealth -= amount;
	}
}

int ABloodInvadersPlayer::GetPayerHealth()
{
	return PlayerHealth;
}
