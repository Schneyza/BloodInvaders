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
	return 1 * USimplexNoiseBPLibrary::SimplexNoise2D(x * base, y * base)
		+ 0.5 * USimplexNoiseBPLibrary::SimplexNoise2D(x * base * 2 + 1, y * base * 2)
		+ 0.25 * USimplexNoiseBPLibrary::SimplexNoise2D(x * base * 4 - 1, y * base * 4)
		+ 0;
}


FVector UFluidDynamics::getFluidVelocity2D(FVector location)
{
	float ds = 1e-4;
	float V = getFluidPotential(location);
	float dVdx = (getFluidPotential(FVector(location.X + ds / globalScale, location.Y, location.Z)) - V) / ds;
	float dVdy = (getFluidPotential(FVector(location.X, location.Y + ds / globalScale, location.Z)) - V) / ds;
	// divide by global scale to keep velocity invariant under scale transformation
	return FVector(dVdy * globalTurbulenceAmplitude, -dVdx * globalTurbulenceAmplitude, 0) + linearVelocity;
}


FVector UFluidDynamics::getFluidTorque2D(FVector location)
{
	// Only z component = dy/dx - dy/dx = -2 dV/dxdy
	float ds = 1e-4;
	float V = getFluidPotential(location);
	float Vx = getFluidPotential(FVector(location.X + ds / globalScale, location.Y, location.Z));
	float Vy = getFluidPotential(FVector(location.X, location.Y + ds / globalScale, location.Z));
	float Vxy = getFluidPotential(FVector(location.X + ds / globalScale, location.Y + ds / globalScale, location.Z));
	float dVdxdy = (Vxy - Vx - Vy + V) / (ds * ds);
	return FVector(0, 0, -2 * dVdxdy);
}

void UFluidDynamics::MoveWithFluid(UPrimitiveComponent* target, float factor)
{
	FVector fluidV = getFluidVelocity2D(target->GetComponentLocation());
	target->SetPhysicsLinearVelocity(fluidV * factor);
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


void UFluidDynamics::ApplyFluidTorque(UPrimitiveComponent* target)
{
	FVector fluidT = getFluidTorque2D(target->GetComponentLocation());
	FVector objectT = target->GetPhysicsAngularVelocity();
	FVector dT = fluidT - objectT;
	float mass = target->GetMass();
	FVector torque = dT * fluidInteractionStrength * FGenericPlatformMath::Pow(mass, 0.66667f);
	target->AddTorque(torque);
}

void UFluidDynamics::ApplyFluid(UPrimitiveComponent* target)
{
	ApplyFluidForce(target);
	ApplyFluidTorque(target);
}