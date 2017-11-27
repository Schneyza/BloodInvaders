// Fill out your copyright notice in the Description page of Project Settings.

#include "BloodInvadersPlayer.h"
#include "Projectile.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

const FName ABloodInvadersPlayer::MoveForwardBinding("MoveForward");
const FName ABloodInvadersPlayer::MoveRightBinding("MoveRight");
const FName ABloodInvadersPlayer::FireBinding("Fire");

bool bFiring = false;

ABloodInvadersPlayer::ABloodInvadersPlayer()
{
	// Create the mesh component
	PlayerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMesh"));
	RootComponent = PlayerMeshComponent;
	PlayerMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when player does
	CameraBoom->TargetArmLength = 1200.f;
	CameraBoom->RelativeRotation = FRotator(-80.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

										  // Create a camera...
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;	// Camera does not rotate relative to arm

														// Movement
	MoveSpeed = 1000.0f;
	// Weapon
	GunOffset = FVector(65.f, 0.f, 0.f);
	FireRate = 0.1f;
	bCanFire = true;
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
		//const FRotator NewRotation = Movement.Rotation();
		const FRotator NewRotation = FRotator(0.0f, 90.0f, 0.0f);
		FHitResult Hit(1.f);
		RootComponent->MoveComponent(Movement, NewRotation, true, &Hit);

		if (Hit.IsValidBlockingHit())
		{
			const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
			const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
			RootComponent->MoveComponent(Deflection, NewRotation, true);
		}
	}

	// Create fire direction vector
	FireShot();
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
			}

			bCanFire = false;
			World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &ABloodInvadersPlayer::ShotTimerExpired, FireRate);

			// try and play the sound if specified
			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}

			bCanFire = false;

		}

	}
}

void ABloodInvadersPlayer::EnableFiring() {
	bFiring = true;
}

void ABloodInvadersPlayer::DisableFiring() {
	bFiring = false;
}

void ABloodInvadersPlayer::ShotTimerExpired()
{
	bCanFire = true;
}

