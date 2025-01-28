// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterComponents/PlayerAbilitiesComponent.h"
#include <Interfaces/DeflectableInterface.h>
#include <Interfaces/GrabbableObject.h>

// Sets default values for this component's properties
UPlayerAbilitiesComponent::UPlayerAbilitiesComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerAbilitiesComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UPlayerAbilitiesComponent::PerformBulletTrace()
{
	FCollisionShape Capsule{
		FCollisionShape::MakeCapsule(CapsuleRadius, HalfHeight)
	};

	FCollisionQueryParams IgnoreParams{
		FName{ TEXT("Ignore Params") },
		true,
		GetOwner()
	};

	TArray<FHitResult> OutResults;

	/*bool bHasFoundTargets{ GetWorld()->SweepMultiByChannel(
		OutResults,

	) };*/
}

void UPlayerAbilitiesComponent::HandleBulletTraceResults(const TArray<FHitResult>& HitResults)
{
	if (HitResults.Num() <= 0) { return; }

	for (const FHitResult& Hit : HitResults)
	{
		AActor* HitActor = Hit.GetActor();

		if (!HitActor || !HitActor->Implements<UDeflectableInterface>())
		{
			continue;
		}
		
		IDeflectableInterface* Bullet = Cast<IDeflectableInterface>(HitActor);
		Bullet->FreezeBullet();
	}

}

void UPlayerAbilitiesComponent::GrabObject(UCustomHandPoseRecognizer* PoseClass, AActor* GrabbedActor, UCustomXRHandComponent* GrabHand, FName SocketName)
{
	if (!GrabbedActor->Implements<UGrabbableObject>()) { return; }

	/*CurrentGrabbedActor = GrabbedActor;*/

	if (!GrabbedActor) { return; }

	IGrabbableObject* GrabbableInterface = Cast<IGrabbableObject>(GrabbedActor);

	if (!GrabbableInterface->IsGrabbed())
	{
		OnGrabbedDelegate.Broadcast(PoseClass);

		GrabbableInterface->Execute_OnGrabbed(GrabbedActor, GrabHand, SocketName);

	}

}

void UPlayerAbilitiesComponent::ReleaseGrabbedActor(AActor* ActorToRelease)
{
	if (ActorToRelease && ActorToRelease->Implements<UGrabbableObject>())
	{
		IGrabbableObject::Execute_OnReleased(ActorToRelease);
		
	}
}


// Called every frame
void UPlayerAbilitiesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

