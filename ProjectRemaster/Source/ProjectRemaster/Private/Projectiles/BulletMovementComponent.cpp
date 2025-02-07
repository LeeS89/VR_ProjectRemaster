// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/BulletMovementComponent.h"

UBulletMovementComponent::UBulletMovementComponent()
{

}

void UBulletMovementComponent::DeflectBullet(const FVector& NewNormal)
{
	
	float VelocityLength = StoredVelocity.Size();
	float NewVelocityLength = VelocityLength * SpeedMultiplier;

	Velocity = NewNormal * NewVelocityLength;
}

void UBulletMovementComponent::InitializeMovement(const FRotator& SpawnRotation)
{
	FVector NewVelocity{ SpawnRotation.Vector() * InitialSpeed };
	Velocity = NewVelocity;
	StoredVelocity = Velocity;
}
