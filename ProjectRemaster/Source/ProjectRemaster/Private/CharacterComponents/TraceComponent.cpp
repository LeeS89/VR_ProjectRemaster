// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/Character.h"
#include "Interfaces/MainPlayer.h"
#include "Enums/EHand.h"
#include "Structs/FTraceSockets.h"
#include "CharacterComponents/CustomXRHandComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CharacterComponents/TraceComponent.h"

// Sets default values for this component's properties
UTraceComponent::UTraceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTraceComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* CharacterRef{ GetOwner<ACharacter>() };

	if (!IsValid(CharacterRef)) { return; }

	if (!CharacterRef->Implements<UMainPlayer>()) { return; }

	IPlayerRef = Cast<IMainPlayer>(CharacterRef);

	InitializeHands();
	
}

void UTraceComponent::InitializeHands()
{
	TArray<UCustomXRHandComponent*> HandComponents = IPlayerRef->GetHandComponents();

	for (UCustomXRHandComponent* HandComponent : HandComponents)
	{
		if (HandComponent)
		{
			EHand CurrentHandType = HandComponent->GrabHandSide;

			if (CurrentHandType == EHand::Left)
			{
				LeftHandGrabComp = HandComponent;
			}
			else if (CurrentHandType == EHand::Right)
			{
				RightHandGrabComp = HandComponent;
			}
		}
	}
}


void UTraceComponent::PerformGrabTrace(TEnumAsByte<EHand> HandToTrace)
{
	if (HandToTrace == EHand::Left)
	{
		CurrentGrabComp = LeftHandGrabComp;
	}
	else if (HandToTrace == EHand::Right)
	{
		CurrentGrabComp = RightHandGrabComp;
	}

	if (!IsValid(CurrentGrabComp)) { return; }

	for (const TPair<TEnumAsByte<EHand>, FTraceSockets>& Pair : HandSockets)
	{
		TEnumAsByte<EHand> Hand = Pair.Key;
		FTraceSockets Sockets = Pair.Value;

		if (Hand != HandToTrace)
		{
			continue;
		}
		FVector SocketLocation{ CurrentGrabComp->GetSocketLocation(Sockets.Start) };
		FQuat SocketRotation{ CurrentGrabComp->GetSocketQuaternion(Sockets.Rotation) };

		FCollisionShape Capsule{
			FCollisionShape::MakeCapsule(CapsuleRadius,CapsuleHalfHeight)
		};

		FCollisionQueryParams IgnoreParams{
			FName{TEXT("Ignore Params")},
			false,
			GetOwner()
		};
		TArray<FHitResult> OutResults;

		bool bHasFoundTargets{ GetWorld()->SweepMultiByChannel(
			OutResults,
			SocketLocation,
			SocketLocation,
			SocketRotation,
			ECollisionChannel::ECC_GameTraceChannel1,
			Capsule,
			IgnoreParams
		) };

		if (bHasFoundTargets)
		{
			OnGrabDelegate.Broadcast(Hand);
		}

		if (!bDebugMode) { return; }

		FVector CenterPoint{
				UKismetMathLibrary::VLerp(
					SocketLocation, SocketLocation, 0.5f
				)
		};

		UKismetSystemLibrary::DrawDebugCapsule(
			GetWorld(),
			CenterPoint,
			Capsule.GetCapsuleHalfHeight(),
			Capsule.GetCapsuleRadius(),
			SocketRotation.Rotator(),
			bHasFoundTargets ? FLinearColor::Green : FLinearColor::Red,
			0.5f,
			2.0f


		);



	}
	CurrentGrabComp = nullptr;
	//FVector SocketLocation{ RightHandGrabComp->GetSocketLocation(Socket.Start) };

}


// Called every frame
void UTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//TArray<FHitResult> AllResults;

	if (!IsValid(RightHandGrabComp) || !IsValid(LeftHandGrabComp)) { return; }



	//for (const FTraceSockets Socket : Sockets)
	//{
	//	if (Socket.GrabbingHand == EHand::Right)
	//	{
	//		FVector SocketLocation{ RightHandGrabComp->GetSocketLocation(Socket.Start) };
	//		//FVector EndLocation = SocketLocation + RightHandGrabComp->GetForwardVector() * TraceDistance;
	//		FQuat SocketRotation{ RightHandGrabComp->GetSocketQuaternion(Socket.Rotation) };

	//		FCollisionShape Capsule{
	//			FCollisionShape::MakeCapsule(CapsuleRadius,CapsuleHalfHeight)
	//		};

	//		FCollisionQueryParams IgnoreParams{
	//			FName{TEXT("Ignore Params")},
	//			false,
	//			GetOwner()
	//		};
	//		TArray<FHitResult> OutResults;


	//		bool bHasFoundTargets{ GetWorld()->SweepMultiByChannel(
	//			OutResults,
	//			SocketLocation,
	//			SocketLocation,
	//			SocketRotation,
	//			ECollisionChannel::ECC_GameTraceChannel1,
	//			Capsule,
	//			IgnoreParams
	//		) };

	//		if (!bDebugMode) { return; }

	//		FVector CenterPoint{
	//				UKismetMathLibrary::VLerp(
	//					SocketLocation, SocketLocation, 0.5f
	//				)
	//		};

	//		UKismetSystemLibrary::DrawDebugCapsule(
	//			GetWorld(),
	//			CenterPoint,
	//			Capsule.GetCapsuleHalfHeight(),
	//			Capsule.GetCapsuleRadius(),
	//			SocketRotation.Rotator(),
	//			bHasFoundTargets ? FLinearColor::Green : FLinearColor::Red,
	//			0.5f,
	//			2.0f
	//			//bHasFoundTargets ? FLinearColor::Green : FLinearColor::Red,
	//		);
	//	}
	//	else if (Socket.GrabbingHand == EHand::Left)
	//	{
	//		FVector SocketLocation{ LeftHandGrabComp->GetSocketLocation(Socket.Start) };
	//		//FVector EndLocation = SocketLocation + RightHandGrabComp->GetForwardVector() * TraceDistance;
	//		FQuat SocketRotation{ LeftHandGrabComp->GetSocketQuaternion(Socket.Rotation) };

	//		FCollisionShape Capsule{
	//			FCollisionShape::MakeCapsule(CapsuleRadius,CapsuleHalfHeight)
	//		};

	//		FCollisionQueryParams IgnoreParams{
	//			FName{TEXT("Ignore Params")},
	//			false,
	//			GetOwner()
	//		};
	//		TArray<FHitResult> OutResults;


	//		bool bHasFoundTargets{ GetWorld()->SweepMultiByChannel(
	//			OutResults,
	//			SocketLocation,
	//			SocketLocation,
	//			SocketRotation,
	//			ECollisionChannel::ECC_GameTraceChannel1,
	//			Capsule,
	//			IgnoreParams
	//		) };

	//		if (!bDebugMode) { return; }

	//		FVector CenterPoint{
	//				UKismetMathLibrary::VLerp(
	//					SocketLocation, SocketLocation, 0.5f
	//				)
	//		};

	//		UKismetSystemLibrary::DrawDebugCapsule(
	//			GetWorld(),
	//			CenterPoint,
	//			Capsule.GetCapsuleHalfHeight(),
	//			Capsule.GetCapsuleRadius(),
	//			SocketRotation.Rotator(),
	//			bHasFoundTargets ? FLinearColor::Green : FLinearColor::Red,
	//			0.5f,
	//			2.0f
	//			//bHasFoundTargets ? FLinearColor::Green : FLinearColor::Red,
	//		);
	//	}

	//	/*if (bHasFoundTargets)
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("GRABBALE OBJECT DETECTED!!!"));
	//	}*/
	//}

	
}



