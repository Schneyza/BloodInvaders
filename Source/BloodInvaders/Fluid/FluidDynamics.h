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
	static FVector linearVelocity;
	static float globalScale;
	static float globalTurbulenceAmplitude;
	static float fluidInteractionStrength;

public:

	// Settings

	UFUNCTION(BlueprintCallable, Category = "FluidDynamics")
		static void setTurbulenceScale(float scale);

	UFUNCTION(BlueprintCallable, Category = "FluidDynamics")
		static void setLinearVelocity(FVector linearVelocity);

	UFUNCTION(BlueprintCallable, Category = "FluidDynamics")
		static void setTurbulenceAmplitude(float amplitude);

	UFUNCTION(BlueprintCallable, Category = "FluidDynamics")
		static void setInteractionStrength(float factor);


	// Calculate field values

	UFUNCTION(BlueprintCallable, Category = "FluidDynamics")
		static float getFluidPotential(FVector location);

	UFUNCTION(BlueprintCallable, Category = "FluidDynamics")
		static FVector getFluidVelocity2D(FVector location);

	UFUNCTION(BlueprintCallable, Category = "FluidDynamics")
		static FVector getFluidTorque2D(FVector location);


	// Apply physics

	UFUNCTION(BlueprintCallable, Category = "FluidDynamics")
		static void MoveWithFluid(UPrimitiveComponent* target);

	UFUNCTION(BlueprintCallable, Category = "FluidDynamics")
		static void ApplyFluidForce(UPrimitiveComponent* target);

	UFUNCTION(BlueprintCallable, Category = "FluidDynamics")
		static void ApplyFluidTorque(UPrimitiveComponent* target);

	UFUNCTION(BlueprintCallable, Category = "FluidDynamics")
		static void ApplyFluid(UPrimitiveComponent* target);
};
