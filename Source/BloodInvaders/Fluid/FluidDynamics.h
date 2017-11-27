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


	UFUNCTION(BlueprintCallable, Category = "FluidDynamics")
		static void setTurbulenceScale(float scale);

	UFUNCTION(BlueprintCallable, Category = "FluidDynamics")
		static void setLinearVelocity(FVector linearVelocity);

	UFUNCTION(BlueprintCallable, Category = "FluidDynamics")
		static void setTurbulenceAmplitude(float amplitude);

	UFUNCTION(BlueprintCallable, Category = "FluidDynamics")
		static void setInteractionStrength(float factor);


	UFUNCTION(BlueprintCallable, Category = "FluidDynamics")
		static float getFluidPotential(FVector vec);

	UFUNCTION(BlueprintCallable, Category = "FluidDynamics")
		static FVector getFluidVelocity2D(FVector vec);

	UFUNCTION(BlueprintCallable, Category = "FluidDynamics")
		static void MoveWithFluid(UPrimitiveComponent* target);

	UFUNCTION(BlueprintCallable, Category = "FluidDynamics")
		static void ApplyFluidForce(UPrimitiveComponent* target);
};
