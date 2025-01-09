// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterComponents/WeaponTraceComponent.h"
#include <Kismet/GameplayStatics.h>
#include <Interfaces/DeflectableInterface.h>


#include "Sound/SoundBase.h"

// Sets default values for this component's properties
UWeaponTraceComponent::UWeaponTraceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UWeaponTraceComponent::BeginPlay()
{
	Super::BeginPlay();

	StaticMeshComp = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
	
}


// Called every frame
void UWeaponTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UWeaponTraceComponent::HandleTraceResults(const TArray<FHitResult>& HitResults)
{
	if (HitResults.Num() <= 0) { return; }

	for (const FHitResult& Hit : HitResults)
	{
		AActor* HitActor = Hit.GetActor();
		if (!HitActor)
		{
			continue;
		}

		if (HitActor->Implements<UDeflectableInterface>())
		{
			DeflectInterface = Cast<IDeflectableInterface>(HitActor);

			if (DeflectInterface && !DeflectInterface->GetDeflectionHasBeenProcessed())
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeflectSound, Hit.ImpactPoint);
				DeflectInterface->Execute_OnDeflected(HitActor, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
				HitActor->SetInstigator(GetOwner()->GetInstigator());
			}
		}
	/*	else if (Hit.GetActor()->Implements<UOverlappableInterface>())
		{

			OverlapInterface = Cast<IOverlappableInterface>(HitActor);

			OverlapInterface->Execute_OnLightsaberOverlapping(HitActor);
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 10.0f, 12, FColor::Blue, false);
		}*/
		else
		{
			OnOverlappingDelegate.Broadcast(HitActor, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
		}
	}
	
}

void UWeaponTraceComponent::SetTraceLocationAndRotation()
{
	Super::SetTraceLocationAndRotation();
}
	


#pragma region Redundant



//void UWeaponTraceComponent::PerformTrace()
//{
//	FVector StartSocketLocation{ StaticMeshComp->GetSocketLocation(Sockets.Start) };
//	FVector EndSocketLocation{ StaticMeshComp->GetSocketLocation(Sockets.End) };
//	FQuat SocketRotation{ StaticMeshComp->GetSocketQuaternion(Sockets.Rotation) };
//
//
//
//	FCollisionShape Capsule{
//		FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight)
//	};
//
//	FCollisionQueryParams IgnoreParams{
//		FName{ TEXT("Ignore Params") },
//		true,
//		GetOwner()
//	};
//
//
//	TArray<FHitResult> OutResults;
//
//	bool bHasFoundTargets{ GetWorld()->SweepMultiByChannel(
//		OutResults,
//		StartSocketLocation,
//		EndSocketLocation,
//		FQuat::Identity,
//		TraceChannel,
//		Capsule,
//		IgnoreParams
//	) };
//
//
//	if (bHasFoundTargets)
//	{
//		for (const FHitResult& Hit : OutResults)
//		{
//			AActor* HitActor = Hit.GetActor();
//			if (!HitActor)
//			{
//				continue;
//			}
//
//			if (HitActor->Implements<UDeflectableInterface>())
//			{
//				UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeflectSound, Hit.ImpactPoint);
//
//				DeflectInterface = Cast<IDeflectableInterface>(HitActor);
//
//				DeflectInterface->Execute_OnDeflected(HitActor);
//			}
//			else if (Hit.GetActor()->Implements<UOverlappableInterface>())
//			{
//
//				OverlapInterface = Cast<IOverlappableInterface>(HitActor);
//
//				OverlapInterface->Execute_OnLightsaberOverlapping(HitActor);
//				DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 10.0f, 12, FColor::Blue, false);
//			}
//			else
//			{
//				OnOverlappingDelegate.Broadcast(HitActor, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
//			}
//		}
//		/*if (!ActiveParticleSystem)
//		{
//		ActiveParticleSystem = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SparkParticles, OutResult.Location, OutResult.ImpactNormal.Rotation());
//		}*/
//	}
//	else
//	{
//		/*if (ActiveParticleSystem)
//		{
//
//		ActiveParticleSystem->DestroyComponent();
//		ActiveParticleSystem = nullptr;
//		}*/
//	}
//
//	/*if (ActiveParticleSystem)
//	{
//	ActiveParticleSystem->SetWorldLocation(OutResult.Location);
//	ActiveParticleSystem->SetWorldRotation(OutResult.ImpactNormal.Rotation());
//	}*/
//	///////////////////////////
//	// Trace Hit Logic Here
//	///////////////////////////
//	if (bIsDebugMode)
//	{
//
//		FVector CenterPoint{
//			UKismetMathLibrary::VLerp(
//				StartSocketLocation, EndSocketLocation, 0.5f
//			)
//		};
//
//		//FVector CenterPoint{ (StartSocketLocation + EndSocketLocation) / 2.0f };
//		float CapsuleHeight = CenterPoint.Size() / 2.0f;
//
//		UKismetSystemLibrary::DrawDebugCapsule(
//			GetWorld(),
//			CenterPoint,
//			Capsule.GetCapsuleAxisHalfLength(),
//			Capsule.GetCapsuleRadius(),
//			SocketRotation.Rotator(),
//			bHasFoundTargets ? FLinearColor::Yellow : FLinearColor::Black,
//			0.0f,
//			1.0f
//		);
//
//	}
//}
#pragma endregion
