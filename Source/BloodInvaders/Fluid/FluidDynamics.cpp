// Fill out your copyright notice in the Description page of Project Settings.

#include "FluidDynamics.h"
#include "SimplexNoiseBPLibrary.h"


FVector UFluidDynamics::coordinateOffset = FVector(0, 0, 0);
FVector UFluidDynamics::linearVelocity = FVector(100, 0, 0);
float UFluidDynamics::globalScale = 1e-3;
float UFluidDynamics::globalTurbulenceAmplitude = 10;
float UFluidDynamics::fluidInteractionStrength = 1.0;
// Domain
int UFluidDynamics::domainMode = 0;
float UFluidDynamics::domainFalloffDistance = 100;
FVector UFluidDynamics::domainBot = FVector();
FVector UFluidDynamics::domainTop = FVector();

void UFluidDynamics::setTurbulenceScale(float scale)
{
	globalScale = scale;
}

void UFluidDynamics::setLinearVelocity(FVector v)
{
	linearVelocity = v;
}

void UFluidDynamics::setCoordinateOffset(FVector offset)
{
	coordinateOffset = offset;
}

void UFluidDynamics::setTurbulenceAmplitude(float amplitude)
{
	globalTurbulenceAmplitude = amplitude;
}

void UFluidDynamics::setInteractionStrength(float factor)
{
	fluidInteractionStrength = factor;
}


void UFluidDynamics::setBoxDomain(FVector center, FVector extents, float falloffDistance)
{
	domainMode = 1;
	domainBot = center - extents / 2;
	domainTop = center + extents / 2;
	domainFalloffDistance = falloffDistance;
}

void UFluidDynamics::clearDomain()
{
	domainMode = 0;
}


float UFluidDynamics::getFluidPotential(FVector vec)
{
	float x = vec.X + coordinateOffset.X;
	float y = vec.Y + coordinateOffset.Y;
	float base = globalScale;
	float insidePotential = 1 * USimplexNoiseBPLibrary::SimplexNoise2D(x * base, y * base)
		+ 0.5 * USimplexNoiseBPLibrary::SimplexNoise2D(x * base * 2 + 1, y * base * 2)
		+ 0.25 * USimplexNoiseBPLibrary::SimplexNoise2D(x * base * 4 - 1, y * base * 4)
		+ 0;
	if (domainMode == 0) return insidePotential;
	else {
		if (vec.X < domainBot.X || vec.X > domainTop.X
			|| vec.Y < domainBot.Y || vec.Y > domainTop.Y
			|| vec.Z < domainBot.Z || vec.Z > domainTop.Z)
			return 0;
		float nearestDistance = 
			FMath::Min(FMath::Min(vec.X-domainBot.X, domainTop.X-vec.X),
			FMath::Min(FMath::Min(vec.Y - domainBot.Y, domainTop.Y - vec.Y),
				FMath::Min(vec.Z - domainBot.Z, domainTop.Z - vec.Z)));
		if(nearestDistance > domainFalloffDistance)
			return insidePotential;
		else {
			float r = nearestDistance / domainFalloffDistance;
			return insidePotential * (15/8.0*r - 10/8.0*r*r*r + 3/8.0*r*r*r*r*r);
		}
	}
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


void UFluidDynamics::ApplyFluidTorque(UPrimitiveComponent* target, bool simulate3D)
{
	FVector fluidT = getFluidTorque2D(target->GetComponentLocation());
	if(simulate3D) {
		FVector fluidV = getFluidVelocity2D(target->GetComponentLocation());
		FVector fluidVz = getFluidVelocity2D(target->GetComponentLocation()+FVector(16/globalScale, 0, 0));
		FVector dVdz = (fluidVz - fluidV);
		fluidT += dVdz * 1;
	}
	FVector objectT = target->GetPhysicsAngularVelocity();
	FVector dT = fluidT - objectT;
	float mass = target->GetMass();
	FVector torque = dT * fluidInteractionStrength * FGenericPlatformMath::Pow(mass, 0.66667f);
	target->AddTorque(torque);
}

void UFluidDynamics::ApplyFluid(UPrimitiveComponent* target)
{
	ApplyFluidForce(target);
	ApplyFluidTorque(target, true);
}