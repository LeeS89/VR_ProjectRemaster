// Fill out your copyright notice in the Description page of Project Settings.


#include "TestClasses(DELETE_AFTER_USE)/BulletTraceComponent.h"
#include "Engine/DamageEvents.h"
//#include "Interfaces/EnemyInterface.h"
//#include "Interfaces/MainPlayer.h"
#include "UtilityClasses/DamageUtility.h"
#include "TestClasses(DELETE_AFTER_USE)/BulletBase.h"



void UBulletTraceComponent::BeginPlay()
{
	Super::BeginPlay();

	StaticMeshComp = GetOwner()->FindComponentByClass<UStaticMeshComponent>();

	OwnerRef = GetOwner<ABulletBase>();
}

void UBulletTraceComponent::HandleTraceResults(const TArray<FHitResult>& HitResults)
{
	if (bHasHit || HitResults.Num() <= 0) { return; }


	if (!OwnerRef || !OwnerRef->Implements<UDeflectableInterface>()) { return; }

	bHasHit = true;
	FHitResult HitResult{ HitResults[0] };
	AActor* HitActor{ HitResult.GetActor() };

	IDeflectableInterface* BulletInterface = Cast<IDeflectableInterface>(OwnerRef);
	BulletInterface->PlayHitParticle(true, HitResult.ImpactPoint, HitResult.ImpactPoint.Rotation());


	APawn* InstigatorPawn = GetOwner()->GetInstigator();
	if (!InstigatorPawn) { return; }

	

	if (UDamageUtility::ShouldDamageActor(InstigatorPawn, HitActor))
	{
		DamageTarget(HitActor);
	}

	
	BulletInterface->OnExpired();

}

void UBulletTraceComponent::SetTraceLocationAndRotation()
{
	if (!StaticMeshComp) { return; }

	if (PreviousLocation.IsZero())
	{
		// Initialize PreviousLocation to the starting socket's position on the first frame
		PreviousLocation = StaticMeshComp->GetSocketLocation(Sockets.Start);
	}

	// StartLocation is the previous frame's position
	FVector StartLocation = PreviousLocation;

	// EndLocation is the current frame's socket end position
	FVector EndLocation = StaticMeshComp->GetSocketLocation(Sockets.End);

	// Perform the trace from the last position to the current end position
	PerformTrace(StartLocation, EndLocation, FQuat::Identity);

	// Update PreviousLocation for the next frame
	PreviousLocation = StartLocation;
}



void UBulletTraceComponent::DamageTarget(AActor* ActorToDamage)
{
	FDamageEvent TargetAttackedEvent;
	float Damage{ 0.0f };

	IDeflectableInterface* BulletRef = Cast<IDeflectableInterface>(OwnerRef);
	if (BulletRef)
	{
		Damage = BulletRef->GetDamage();

		ActorToDamage->TakeDamage(
			Damage,
			TargetAttackedEvent,
			GetOwner()->GetInstigatorController(),
			GetOwner()
		);
	}
}
