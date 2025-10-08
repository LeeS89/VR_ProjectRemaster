// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OculusXRInputFunctionLibrary.h"
#include "Enums/ETraceType.h"
#include "TraceComponent.generated.h"


class UCustomXRHandComponent;

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_ThreeParams(
	FOnTryGrabSignature,
	UTraceComponent, OnTryGrabDelegate,
	class UCustomHandPoseRecognizer*, PoseClass,
	AActor*, ActorToGrab,
	UCustomXRHandComponent*, GrabbingHand
);



DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FOnFreezeSignature,
	UTraceComponent, OnFreezeDelegate,
	TArray<FHitResult>, OutResults
);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTREMASTER_API UTraceComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	
	UFUNCTION()
	void SetupTraceParams(
		TEnumAsByte<ETraceType> TraceType,
		TEnumAsByte<ECollisionChannel> Channel,
		UCustomXRHandComponent* GrabHand,
		class UCustomHandPoseRecognizer* PoseClass,
		const FVector& Start = FVector::ZeroVector,
		const FVector& End = FVector::ZeroVector, const FRotator& Rot = FRotator::ZeroRotator,
		float ShapeRadius = 0.0f, float HalfHeight = 0.0f, bool bDebugVisual = false
	);

	

	UFUNCTION()
	void PerformSingleTrace(
		TEnumAsByte<ECollisionChannel> Channel,
		UCustomXRHandComponent* GrabHand,
		class UCustomHandPoseRecognizer* PoseClass,
		const FVector& Start = FVector::ZeroVector,
		const FVector& End = FVector::ZeroVector, const FRotator& Rot = FRotator::ZeroRotator,
		float ShapeRadius = 0.0f, float HalfHeight = 0.0f, bool bDebugVisual = false

	);

	// Moving to Abilities Component - Broadcasting Event Here Instead
	UFUNCTION()
	void HandleGrabResult(class UCustomHandPoseRecognizer* PoseClass, AActor* ActorToGrab, UCustomXRHandComponent* GrabHand);

	UFUNCTION()
	void PerformMultiTrace(
		TEnumAsByte<ECollisionChannel> Channel,
		const FVector& Start = FVector::ZeroVector,
		const FVector& End = FVector::ZeroVector, const FRotator& Rot = FRotator::ZeroRotator,
		float ShapeRadius = 0.0f, bool bDebugVisual = false
	);

public:	
	// Sets default values for this component's properties
	UTraceComponent();

	//Redundant
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CapsuleHalfHeight{ 8.0f };

	//Redundant
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CapsuleRadius{ 4.0f };

	//Redundant
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SphereRadius{ 4.0f };

	
	UFUNCTION(BlueprintCallable)
	void PerformTrace(TEnumAsByte<ETraceType> TraceType,
		TEnumAsByte<ECollisionChannel> Channel = ECollisionChannel::ECC_Visibility,
		UCustomXRHandComponent* GrabHand = nullptr,
		class UCustomHandPoseRecognizer* PoseClass = nullptr,
		const FVector& Start = FVector::ZeroVector, 
		const FVector& End = FVector::ZeroVector, const FRotator& Rot = FRotator::ZeroRotator,
		float ShapeRadius = 5.0f, float HalfHeight = 0.0f, bool bDebugVisual = false
		);

	
	UPROPERTY(BlueprintAssignable)
	FOnTryGrabSignature OnTryGrabDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnFreezeSignature OnFreezeDelegate;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
