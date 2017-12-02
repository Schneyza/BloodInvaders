// Fill out your copyright notice in the Description page of Project Settings.

#include "Virus.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/CollisionProfile.h"


const FName AVirus::InfectBinding("Infect");

AVirus::AVirus()
{
<<<<<<< Updated upstream
	// Create the Sphere Component
	PlayerSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerSphere"));
	RootComponent = PlayerSphereComponent;
	PlayerSphereComponent->SetSphereRadius(1.f);
	PlayerSphereComponent->SetHiddenInGame(true);
	PlayerSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
=======
	// Create the mesh component
	PlayerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMesh"));
	PlayerMeshComponent->OnComponentHit.AddDynamic(this, &AVirus::OnHit);		// set up a notification for when this component hits something

	RootComponent = PlayerMeshComponent;
	PlayerMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
>>>>>>> Stashed changes
}

void AVirus::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	ABloodInvadersPlayer::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(InfectBinding, IE_Repeat, this, &AVirus::TryInfect);
	PlayerInputComponent->BindAction(InfectBinding, IE_Released, this, &AVirus::TryInfect);
}

void AVirus::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogClass, Log, TEXT("Overlap: %i"), infectableCells.size());
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		if (OtherActor->Tags.Contains("Infect"))
		{
			infectableCells.push_back(OtherActor);
		}
	}
}

void AVirus::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		if (OtherActor->Tags.Contains("Infect"))
		{
			infectableCells.remove(OtherActor);
		}
	}
}

void AVirus::TryInfect() {
	UE_LOG(LogClass, Log, TEXT("Player with ControllerId %i started infecting"), ControllerId);
	UE_LOG(LogClass, Log, TEXT("list: %i"), infectableCells.size());

	if (infectableCells.size() > 0) {
		AActor* actor = infectableCells.front();
		this->AttachRootComponentToActor(actor);
		this->SetActorRelativeLocation(FVector(0, 0, 0));
	}
}

void AVirus::BeginPlay()
{
	Super::BeginPlay();
	// Enable Generating hit events on collision
<<<<<<< Updated upstream
	//PlayerMeshComponent->SetNotifyRigidBodyCollision(true);
=======
	PlayerMeshComponent->SetNotifyRigidBodyCollision(true);

	PlayerMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AVirus::OnOverlapBegin);
	PlayerMeshComponent->OnComponentEndOverlap.AddDynamic(this, &AVirus::OnOverlapEnd);
>>>>>>> Stashed changes
}

void AVirus::Tick(float DeltaSeconds)
{
	/* Move the character*/
	Move(DeltaSeconds);

	/* Move the viruses*/
	MoveViruses();

	/* Try to fire a shot*/
	Super::FireShot();
}

/* Moves the character. Use this function to specify custom movement behaviour for the virus*/
void AVirus::Move(float DeltaSeconds)
{
	Super::Move(DeltaSeconds);
}



