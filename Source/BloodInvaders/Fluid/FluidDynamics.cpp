// Fill out your copyright notice in the Description page of Project Settings.

#include "FluidDynamics.h"
#include "SimplexNoiseBPLibrary.h"



FVector UFluidDynamics::linearVelocity = FVector(100, 0, 0);
float UFluidDynamics::globalScale = 1e-3;
float UFluidDynamics::globalTurbulenceAmplitude = 10;
float UFluidDynamics::fluidInteractionStrength = 1.0;



void UFluidDynamics::setTurbulenceScale(float scale)
{
	globalScale = scale;
}

void UFluidDynamics::setLinearVelocity(FVector v)
{
	linearVelocity = v;
}

void UFluidDynamics::setTurbulenceAmplitude(float amplitude)
{
	globalTurbulenceAmplitude = amplitude;
}

void UFluidDynamics::setInteractionStrength(float factor)
{
	fluidInteractionStrength = factor;
}


float UFluidDynamics::getFluidPotential(FVector vec)
{
	float x = vec.X;
	float y = vec.Y;
	float base = globalScale;
	return 1 * USimplexNoiseBPLibrary::SimplexNoise2D(x * base, y * base);
		//+ 0.5 * USimplexNoiseBPLibrary::SimplexNoise2D(x * base * 2 + 1, y * base * 2)
		//+ 0.25 * USimplexNoiseBPLibrary::SimplexNoise2D(x * base * 4 - 1, y * base * 4)
		//+ 0.125 * USimplexNoiseBPLibrary::SimplexNoise2D(x * base * 8, y * base * 8);
}


FVector UFluidDynamics::getFluidVelocity2D(FVector location)
{
	float ds = 1e-4;
	float V = getFluidPotential(location);
	float dVdx = (getFluidPotential(FVector(location.X + ds, location.Y, location.Z)) - V) / ds / globalScale;
	float dVdy = (getFluidPotential(FVector(location.X, location.Y + ds, location.Z)) - V) / ds / globalScale;
	return FVector(dVdy * globalTurbulenceAmplitude, -dVdx * globalTurbulenceAmplitude, 0) + linearVelocity;
}

void UFluidDynamics::MoveWithFluid(UPrimitiveComponent* target)
{
	FVector fluidV = getFluidVelocity2D(target->GetComponentLocation());
	target->SetPhysicsLinearVelocity(fluidV);
}


void UFluidDynamics::ApplyFluidForce(UPrimitiveComponent* target)
{
	FVector fluidV = getFluidVelocity2D(target->GetComponentLocation());
	FVector objectV = target->GetPhysicsLinearVelocity();
	FVector dV = fluidV - objectV;
	float mass = target->GetMass();
	FVector force = dV * fluidInteractionStrength * FGenericPlatformMath::Pow(mass, 0.66667f);
	target->AddForce(force);
}