// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterComponents/TraceComponent.h"
#include "Interfaces/GrabbableObject.h"
#include "CharacterComponents/CustomXRHandComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "OculusXRInputFunctionLibrary.h"





// Sets default values for this component's properties
UTraceComponent::UTraceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UTraceComponent::BeginPlay()
{
	Super::BeginPlay();

}


void UTraceComponent::PerformTrace(TEnumAsByte<ETraceType> TraceType, TEnumAsByte<ECollisionChannel> Channel, 
	UCustomXRHandComponent* GrabHand, UCustomHandPoseRecognizer* PoseClass, 
	const FVector& Start, const FVector& End, const FRotator& Rot, float ShapeRadius, float HalfHeight, bool bDebugVisual)
{
	if (TraceType == ETraceType::NoTrace) { return; }

	SetupTraceParams(TraceType, Channel, GrabHand, PoseClass, Start, End, Rot, ShapeRadius, HalfHeight, bDebugVisual);
}


void UTraceComponent::SetupTraceParams(TEnumAsByte<ETraceType> TraceType, TEnumAsByte<ECollisionChannel> Channel, 
	UCustomXRHandComponent* GrabHand, UCustomHandPoseRecognizer* PoseClass, 
	const FVector& Start, const FVector& End, const FRotator& Rot, float ShapeRadius, float HalfHeight, bool bDebugVisual)
{
	if (TraceType == ETraceType::NoTrace) { return; }

	switch (TraceType)
	{
	case ETraceType::BulletFreezeTrace:
		PerformMultiTrace(Channel, Start, End, Rot, ShapeRadius, bDebugVisual);
		break;
	case ETraceType::GrabTrace:
		PerformSingleTrace(Channel, GrabHand, PoseClass, Start, End, Rot, ShapeRadius, HalfHeight, bDebugVisual);
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("No Trace type Selected"));
		break;
	}
}

/// <summary>
/// Used for simple traces such as when the player makes a fist,
/// performs a trace on the first frame once fist is recognized,
/// if the trace detects a grabbable object, the player grabs the object, otherwise the player moves instead
/// </summary>
/// <param name="Channel"></param>
/// <param name="GrabHand">The hand used to trigger the trace</param>
/// <param name="PoseClass">The Pose recognizer class i.e. the class the "fist gesture belongs to"</param>
/// <param name="Start"></param>
/// <param name="End"></param>
/// <param name="Rot"></param>
/// <param name="ShapeRadius"></param>
/// <param name="HalfHeight"></param>
/// <param name="bDebugVisual"></param>
void UTraceComponent::PerformSingleTrace(TEnumAsByte<ECollisionChannel> Channel, UCustomXRHandComponent* GrabHand, UCustomHandPoseRecognizer* PoseClass, const FVector& Start, const FVector& End, const FRotator& Rot, float ShapeRadius, float HalfHeight, bool bDebugVisual)
{
	if (Start == FVector::ZeroVector) { return; }

	FCollisionShape Capsule{
		FCollisionShape::MakeCapsule(ShapeRadius,HalfHeight)
	};

	FCollisionQueryParams IgnoreParams{
		FName{TEXT("Ignore Params")},
		false,
		GetOwner()
	};

	FHitResult OutResult;

	bool bHasFoundTargets{ GetWorld()->SweepSingleByChannel(
		OutResult,
		Start,
		End,
		Rot.Quaternion(),
		Channel,
		Capsule,
		IgnoreParams
	) };

	if (bHasFoundTargets)
	{
		if (GrabHand)
		{
			HandleGrabResult(PoseClass, OutResult.GetActor(), GrabHand);
		}
	}

	if (bDebugVisual)
	{

		FVector CenterPoint{
				UKismetMathLibrary::VLerp(
					Start, End, 0.5f
				)
		};

		UKismetSystemLibrary::DrawDebugCapsule(
			GetWorld(),
			CenterPoint,
			Capsule.GetCapsuleHalfHeight(),
			Capsule.GetCapsuleRadius(),
			Rot,
			bHasFoundTargets ? FLinearColor::Green : FLinearColor::Red,
			0.5f,
			2.0f

		);
	}

}

/// <summary>
/// If a grabbable object is dectected during the trace, this 
/// broadcasts the result to any listeners.
/// In this case, the player abilities component to handle the grabbing
/// </summary>
/// <param name="PoseClass"></param>
/// <param name="ActorToGrab"></param>
/// <param name="GrabHand"></param>
void UTraceComponent::HandleGrabResult(UCustomHandPoseRecognizer* PoseClass, AActor* ActorToGrab, UCustomXRHandComponent* GrabHand)
{
	if (!ActorToGrab->Implements<UGrabbableObject>()) { return; }

	OnTryGrabDelegate.Broadcast(PoseClass, ActorToGrab, GrabHand);
	

}

/// <summary>
/// Used for more complex traces that can handle multiple results,
/// such as special abilites like the bullet freeze ability for example.
/// </summary>
/// <param name="Channel"></param>
/// <param name="Start"></param>
/// <param name="End"></param>
/// <param name="Rot"></param>
/// <param name="ShapeRadius"></param>
/// <param name="bDebugVisual"></param>
void UTraceComponent::PerformMultiTrace(TEnumAsByte<ECollisionChannel> Channel, const FVector& Start, 
	const FVector& End, const FRotator& Rot, float ShapeRadius, bool bDebugVisual)
{
	if (Start == FVector::ZeroVector || End == FVector::ZeroVector) { return; }

	FCollisionShape Sphere = FCollisionShape::MakeSphere(ShapeRadius);

	FCollisionQueryParams IgnoreParams{
		FName{TEXT("Ignore Params")},
		false,
		GetOwner()
	};

	TArray<FHitResult> OutResults;

	bool bHasFoundTargets{ GetWorld()->SweepMultiByChannel(
		OutResults,
		Start,
		End,
		Rot.Quaternion(),
		Channel,
		Sphere,
		IgnoreParams
	) };

	if (bHasFoundTargets)
	{
		OnFreezeDelegate.Broadcast(OutResults);
	}

	if (!bDebugVisual) { return; }

	FVector CenterPoint{
			UKismetMathLibrary::VLerp(
				Start, End, 0.5f
			)
	};

	UKismetSystemLibrary::DrawDebugSphere(
		GetWorld(),
		CenterPoint,               
		ShapeRadius,
		12,                        
		bHasFoundTargets ? FLinearColor::Green : FLinearColor::Red, 
		0.0f,                      
		2.0f                       
	);
}


// Called every frame
void UTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

#pragma region Redundant
//void UTraceComponent::PerformGrabTrace(EOculusXRHandType HandToTrace, UCustomHandPoseRecognizer* PoseClass)
//{
//	if (HandToTrace == EOculusXRHandType::HandLeft)
//	{
//		CurrentGrabComp = LeftHandGrabComp;
//	}
//	else if (HandToTrace == EOculusXRHandType::HandRight) // Can refactor later to a single if using ternary operator
//	{
//		CurrentGrabComp = RightHandGrabComp;
//	}
//
//	if (!IsValid(CurrentGrabComp)) { return; }
//
//	for (const TPair<EOculusXRHandType, FTraceSockets>& Pair : HandSockets)
//	{
//		EOculusXRHandType Hand = Pair.Key;
//		FTraceSockets Sockets = Pair.Value;
//
//		if (Hand != HandToTrace)
//		{
//			continue;
//		}
//		FVector SocketLocation{ CurrentGrabComp->GetSocketLocation(Sockets.Start) };
//		FQuat SocketRotation{ CurrentGrabComp->GetSocketQuaternion(Sockets.Rotation) };
//
//		FCollisionShape Capsule{
//			FCollisionShape::MakeCapsule(CapsuleRadius,CapsuleHalfHeight)
//		};
//
//		FCollisionQueryParams IgnoreParams{
//			FName{TEXT("Ignore Params")},
//			false,
//			GetOwner()
//		};
//		//TArray<FHitResult> OutResults;
//		FHitResult OutResult;
//
//		bool bHasFoundTargets{ GetWorld()->SweepSingleByChannel(
//			OutResult,
//			SocketLocation,
//			SocketLocation,
//			SocketRotation,
//			ECollisionChannel::ECC_GameTraceChannel1,
//			Capsule,
//			IgnoreParams
//		) };
//
//		if (bHasFoundTargets)
//		{
//
//			if (OutResult.GetActor()->Implements<UGrabbableObject>())
//			{
//				CurrentGrabbedActor = OutResult.GetActor();
//				IGrabbableObject* GrabbableInterface = Cast<IGrabbableObject>(CurrentGrabbedActor);
//
//				if (!GrabbableInterface->IsGrabbed())
//				{
//					OnGrabDelegate.Broadcast(PoseClass);
//
//					GrabbableInterface->Execute_OnGrabbed(CurrentGrabbedActor, CurrentGrabComp, Sockets.WeaponSocket);
//				}
//			}
//		}
//
//		if (!bDebugMode) { return; }
//
//		FVector CenterPoint{
//				UKismetMathLibrary::VLerp(
//					SocketLocation, SocketLocation, 0.5f
//				)
//		};
//
//		UKismetSystemLibrary::DrawDebugCapsule(
//			GetWorld(),
//			CenterPoint,
//			Capsule.GetCapsuleHalfHeight(),
//			Capsule.GetCapsuleRadius(),
//			SocketRotation.Rotator(),
//			bHasFoundTargets ? FLinearColor::Green : FLinearColor::Red,
//			0.5f,
//			2.0f
//
//		);
//
//	}
//	CurrentGrabComp = nullptr;
//
//
//}

//void UTraceComponent::InitializeHands()
//{
//	TArray<UCustomXRHandComponent*> HandComponents = IPlayerRef->GetHandComponents();
//
//	for (UCustomXRHandComponent* HandComponent : HandComponents)
//	{
//		if (HandComponent)
//		{
//			EHand CurrentHandType = HandComponent->GrabHandSide;
//
//			if (CurrentHandType == EHand::Left)
//			{
//				LeftHandGrabComp = HandComponent;
//			}
//			else if (CurrentHandType == EHand::Right)
//			{
//				RightHandGrabComp = HandComponent;
//			}
//		}
//	}
//}

//////Previously in Tick
//TArray<FHitResult> AllResults;

	//if (!IsValid(RightHandGrabComp) || !IsValid(LeftHandGrabComp)) { return; }



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
///////////// To Here
#pragma endregion




