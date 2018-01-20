// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FluidDynamics.generated.h"

/**
 * Blueprint library for simulating a two-dimensional fluid with turbulence.
 */
UCLASS()
class BLOODINVADERS_API UFluidDynamics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
private:
	static FVector coordinateOffset;
	static FVector linearVelocity;
	static float globalScale;
	static float globalTurbulenceAmplitude;
	static float fluidInteractionStrength;
	static float laminarRegime;
	// Domain
	static int domainMode;
	static float domainFalloffDistance;
	static bool hardWall;

	static FVector domainBot;
	static FVector domainTop;

	static FVector domainCenter;
	static FVector domainDirection;
	static float domainRadius;

	static bool isInDomain(FVector location);
	static FVector pointTowardsDomain(FVector location);
	static float flowRegimeFactor(FVector deltaVelocity);
public:

	// Settings

	UFUNCTION(BlueprintCallable, Category = "FluidDynamics")
		static void setTurbulenceScale(float scale);

	UFUNCTION(BlueprintCallable, Category = "FluidDynamics")
		static void setLinearVelocity(FVector linearVelocity);

	UFUNCTION(BlueprintCallable, Category = "FluidDynamics")
		static void setCoordinateOffset(FVector linearVelocity);

	UFUNCTION(BlueprintCallable, Category = "FluidDynamics")
		static void setTurbulenceAmplitude(float amplitude);

	UFUNCTION(BlueprintCallable, Category = "FluidDynamics")
		static void setInteractionStrength(float factor);

	UFUNCTION(BlueprintCallable, Category = "FluidDynamics")
		static void setLaminarRegime(float maxVelocity);


	// Domain

	UFUNCTION(BlueprintCallable, Category = "FluidDynamics")
		static void setBoxDomain(FVector center, FVector extents);

	UFUNCTION(BlueprintCallable, Category = "FluidDynamics")
		static void setInfCylinderDomain(FVector center, FVector direction, float radius);

	UFUNCTION(BlueprintCallable, Category = "FluidDynamics")
		static void configureDomain(float falloffDistance, bool hardWall);

	UFUNCTION(BlueprintCallable, Category = "FluidDynamics")
		static void clearDomain();


	// Calculate field values

	UFUNCTION(BlueprintCallable, Category = "FluidDynamics")
		static float getFluidPotential(FVector location);

	UFUNCTION(BlueprintCallable, Category = "FluidDynamics")
		static FVector getFluidVelocity2D(FVector location);

	UFUNCTION(BlueprintCallable, Category = "FluidDynamics")
		static FVector getFluidTorque2D(FVector location);


	// Apply physics

	UFUNCTION(BlueprintCallable, Category = "FluidDynamics")
		static void MoveWithFluid(UPrimitiveComponent* target, float factor);

	UFUNCTION(BlueprintCallable, Category = "FluidDynamics")
		static void ApplyFluidForce(UPrimitiveComponent* target);

	UFUNCTION(BlueprintCallable, Category = "FluidDynamics")
		static void ApplyFluidTorque(UPrimitiveComponent* target, bool simulate3D);

	UFUNCTION(BlueprintCallable, Category = "FluidDynamics")
		static void ApplyFluidForceToAllBodiesBelow(USkeletalMeshComponent* target);

	UFUNCTION(BlueprintCallable, Category = "FluidDynamics")
		static void ApplyFluid(UPrimitiveComponent* target);
};
