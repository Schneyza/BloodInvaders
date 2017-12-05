// Fill out your copyright notice in the Description page of Project Settings.

#include "FluidDynamics.h"
#include "SimplexNoiseBPLibrary.h"


FVector UFluidDynamics::coordinateOffset = FVector(0, 0, 0);
FVector UFluidDynamics::linearVelocity = FVector(100, 0, 0);
float UFluidDynamics::globalScale = 1e-3;
float UFluidDynamics::globalTurbulenceAmplitude = 10;
float UFluidDynamics::fluidInteractionStrength = 1.0;
float UFluidDynamics::laminarRegime = 1000;
// Domain
int UFluidDynamics::domainMode = 0; // 0: no domain, 1: box domain, 2: cyllinder domain
float UFluidDynamics::domainFalloffDistance = 100;
bool UFluidDynamics::hardWall = false;

FVector UFluidDynamics::domainBot = FVector();
FVector UFluidDynamics::domainTop = FVector();

FVector UFluidDynamics::domainCenter = FVector();
FVector UFluidDynamics::domainDirection = FVector();
float UFluidDynamics::domainRadius = 0;

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

void UFluidDynamics::setLaminarRegime(float maxVelocity)
{
	laminarRegime = maxVelocity;
}


void UFluidDynamics::setBoxDomain(FVector center, FVector extents)
{
	domainMode = 1;
	domainBot = center - extents / 2;
	domainTop = center + extents / 2;
}

void UFluidDynamics::setInfCylinderDomain(FVector center, FVector direction, float radius)
{
	domainMode = 2;
	domainCenter = center;
	domainDirection = direction;
	domainRadius = radius;
}

void UFluidDynamics::configureDomain(float falloffDistance, bool hard)
{
	domainFalloffDistance = falloffDistance;
	hardWall = hard;
}

void UFluidDynamics::clearDomain()
{
	domainMode = 0;
}

bool UFluidDynamics::isInDomain(FVector vec)
{
	if (domainMode == 0) return true;
	if (domainMode == 1)
		return vec.X >= domainBot.X && vec.X <= domainTop.X
		&& vec.Y >= domainBot.Y && vec.Y <= domainTop.Y;
	else if (domainMode == 2)
		return FMath::PointDistToLine(vec, domainDirection, domainCenter) < domainRadius;
	else return false;
}

FVector UFluidDynamics::pointTowardsDomain(FVector loc)
{
	if (domainMode == 0) return FVector();
	if (domainMode == 1) return FVector();
	if (domainMode == 2) {
		return FMath::ClosestPointOnInfiniteLine(domainCenter, domainCenter+domainDirection, loc) - loc;
	}
	return FVector();
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

	// Calculate location w.r.t. domain boundary
	float nearestDistance = 0;

	if (!isInDomain(vec)) return 0;

	// Box domain
	if(domainMode == 1) {
		nearestDistance = 
			FMath::Min(FMath::Min(vec.X-domainBot.X, domainTop.X-vec.X),
			FMath::Min(vec.Y - domainBot.Y, domainTop.Y - vec.Y));
	}
	// Cyllinder domain
	else if (domainMode == 2) {
		float d = FMath::PointDistToLine(vec, domainDirection, domainCenter);
		if (d > domainRadius) return 0;
		nearestDistance = domainRadius - d;
	}

	// Ramp potential to zero at domain boundary
	if (nearestDistance > domainFalloffDistance)
		return insidePotential;
	else {
		float r = nearestDistance / domainFalloffDistance;
		return insidePotential * (15 / 8.0*r - 10 / 8.0*r*r*r + 3 / 8.0*r*r*r*r*r);
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


float UFluidDynamics::flowRegimeFactor(FVector deltaVelocity)
{
	float v = deltaVelocity.Size();
	if (v <= laminarRegime) return 1;
	else return v / laminarRegime;
}


void UFluidDynamics::ApplyFluidForce(UPrimitiveComponent* target)
{
	FVector location = target->GetComponentLocation();
	FVector objectV = target->GetPhysicsLinearVelocity();
	float mass = target->GetMass();

	if (isInDomain(location)) {
		FVector fluidV = getFluidVelocity2D(location);
		FVector dV = fluidV - objectV;
		FVector force = dV * flowRegimeFactor(dV) * fluidInteractionStrength * FGenericPlatformMath::Pow(mass, 0.66667f);
		target->AddForce(force);
	}
	else {
		FVector direction = pointTowardsDomain(location);
		direction.Normalize();
		if (FVector::DotProduct(direction, objectV) < 0 && hardWall) {
			target->SetPhysicsLinearVelocity(direction*100);
		}
		else {
			FVector v = direction * globalTurbulenceAmplitude;
			target->AddForce(v * flowRegimeFactor(v) * fluidInteractionStrength * FGenericPlatformMath::Pow(mass, 0.66667f));
		}
	}
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
	float rfac = flowRegimeFactor(dT * 0.5 * FGenericPlatformMath::Pow(mass, 0.33333f));
	FVector torque = dT * 4 * rfac * fluidInteractionStrength * FGenericPlatformMath::Pow(mass, 0.66667f);
	target->AddTorque(torque);
}

void UFluidDynamics::ApplyFluid(UPrimitiveComponent* target)
{
	ApplyFluidForce(target);
	ApplyFluidTorque(target, true);
}