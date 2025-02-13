// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterComponents/PlayerAbilitiesComponent.h"
#include <Interfaces/GrabbableObject.h>



// Sets default values for this component's properties
UPlayerAbilitiesComponent::UPlayerAbilitiesComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UPlayerAbilitiesComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UPlayerAbilitiesComponent::HandleBulletTraceResults(const TArray<FHitResult>& HitResults)
{
	if (HitResults.Num() <= 0) { return; }
	//UE_LOG(LogTemp, Error, TEXT("Processed bullets contains: %i"), ProcessedBullets.Num());
	for (const FHitResult& Hit : HitResults)
	{
		AActor* HitActor = Hit.GetActor();

		if (!HitActor || !HitActor->Implements<UDeflectableInterface>() || ProcessedBullets.Contains(HitActor))
		{
			continue;
		}

		ProcessedBullets.Add(HitActor);
		
		IDeflectableInterface* BulletInterface = Cast<IDeflectableInterface>(HitActor);
		BulletInterface->FreezeBullet();
		
	}

}

void UPlayerAbilitiesComponent::FireFrozenBullets()
{
	
	if (ProcessedBullets.Num() <= 0) { return; }
	
	float CurrentDelay = 0.1f;

	TArray<AActor*> FrozenBullets = ProcessedBullets;
	ProcessedBullets.Empty();
	
	for (AActor* Bullet : FrozenBullets)
	{
		if (IDeflectableInterface* FBullet = Cast<IDeflectableInterface>(Bullet))
		{

			FTimerHandle BulletFireTimer;

			GetWorld()->GetTimerManager().SetTimer(
				BulletFireTimer,
				[this, FBullet, Bullet, &FrozenBullets]()
				{
					FBullet->UnFreezeBullet();
				},
				CurrentDelay, false
			);

			CurrentDelay += DelayAmount; 
		}
	}
	FrozenBullets.Empty();

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

