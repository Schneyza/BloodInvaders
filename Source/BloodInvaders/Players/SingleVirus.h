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
	
public:	

//Methods
public:	
	ASingleVirus();

	// Called every frame
	virtual void Tick(float DeltaTime) override;// Sets default values for this actor's properties
	

	/* Function to handle collision with blocking objects*/
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BluePrintImplementableEvent, Category = Fluid)
	void MoveWithFluid();

	UFUNCTION(BluePrintImplementableEvent, Category = Virus)
	void RemoveSingleVirusFromArray();

	///** Returns ProjectileMesh subobject **/
	//FORCEINLINE UStaticMeshComponent* GetVirusMesh() const { return VirusMesh; }

	UFUNCTION(BlueprintPure, Category = Mesh)
	UStaticMeshComponent* GetVirusMeshComponent() const;

	UFUNCTION(BlueprintCallable, Category = Virus)
		void DestroySingleVirus();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
