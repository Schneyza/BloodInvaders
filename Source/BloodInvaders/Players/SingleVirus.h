// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SingleVirus.generated.h"

UCLASS()
class BLOODINVADERS_API ASingleVirus : public AActor
{
	GENERATED_BODY()



	/* The mesh for a SingleVirus*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* VirusMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	float PushBackStrength;
	
public:	

//Methods
public:	
	ASingleVirus();

	// Called every frame
	virtual void Tick(float DeltaTime) override;// Sets default values for this actor's properties
	
	// Corresponding Virus Swarm (AVirus)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	AActor* parentVirusSwarm;

	/* Function to handle collision with blocking objects*/
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintImplementableEvent, Category = Fluid)
	void MoveWithFluid();

	UFUNCTION(BlueprintImplementableEvent)
	void MoveWithPlayer();

	UFUNCTION(BlueprintPure, Category = Mesh)
	UStaticMeshComponent* GetVirusMeshComponent() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
