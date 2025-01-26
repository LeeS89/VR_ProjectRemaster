// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs/FTraceSockets.h"
#include "OculusXRInputFunctionLibrary.h"
#include "Enums/ETraceType.h"
#include "TraceComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FOnGrabSignature,
	UTraceComponent, OnGrabDelegate,
	class UCustomHandPoseRecognizer*, PoseClass
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

	UPROPERTY(VisibleAnywhere)
	FVector BulletTraceLocation;

	UPROPERTY(VisibleAnywhere)
	FQuat BulletTraceRotation;

	UPROPERTY(EditAnywhere)
	TMap<EOculusXRHandType,FTraceSockets > HandSockets;

	class UCustomXRHandComponent* LeftHandGrabComp;
	class UCustomXRHandComponent* RightHandGrabComp;

	class UCustomXRHandComponent* CurrentGrabComp;

	class IMainPlayer* IPlayerRef;

	UPROPERTY(EditAnywhere)
	bool bDebugMode{ false };

	UFUNCTION()
	void SetupTraceParams(
		TEnumAsByte<ETraceType> TraceType,
		TEnumAsByte<ECollisionChannel> Channel = ECollisionChannel::ECC_Visibility,
		EOculusXRHandType HandToTrace = EOculusXRHandType::None,
		class UCustomHandPoseRecognizer* PoseClass = nullptr,
		const FVector& Start = FVector::ZeroVector,
		const FVector& End = FVector::ZeroVector, const FRotator& Rot = FRotator::ZeroRotator,
		float ShapeRadius = 5.0f, float HalfHeight = 0.0f, bool bDebugVisual = false
	);

	

	UFUNCTION()
	void PerformSingleTrace(
		TEnumAsByte<ECollisionChannel> Channel = ECollisionChannel::ECC_Visibility,
		EOculusXRHandType HandToTrace = EOculusXRHandType::None,
		class UCustomHandPoseRecognizer* PoseClass = nullptr,
		const FVector& Start = FVector::ZeroVector,
		const FVector& End = FVector::ZeroVector, const FRotator& Rot = FRotator::ZeroRotator,
		float ShapeRadius = 5.0f, float HalfHeight = 0.0f, bool bDebugVisual = false

	);

	UFUNCTION()
	void PerformMultiTrace(
		TEnumAsByte<ECollisionChannel> Channel = ECollisionChannel::ECC_Visibility,
		const FVector& Start = FVector::ZeroVector,
		const FVector& End = FVector::ZeroVector, const FRotator& Rot = FRotator::ZeroRotator,
		float ShapeRadius = 5.0f, bool bDebugVisual = false
	);

public:	
	// Sets default values for this component's properties
	UTraceComponent();

	AActor* CurrentGrabbedActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CapsuleHalfHeight{ 8.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CapsuleRadius{ 4.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SphereRadius{ 4.0f };

	UFUNCTION(BlueprintCallable)
	void PerformGrabTrace(EOculusXRHandType HandToTrace, class UCustomHandPoseRecognizer* PoseClass);

	

	UFUNCTION(BlueprintCallable)
	void PerformTrace(TEnumAsByte<ETraceType> TraceType,
		TEnumAsByte<ECollisionChannel> Channel = ECollisionChannel::ECC_Visibility,
		EOculusXRHandType HandToTrace = EOculusXRHandType::None, 
		class UCustomHandPoseRecognizer* PoseClass = nullptr,
		const FVector& Start = FVector::ZeroVector, 
		const FVector& End = FVector::ZeroVector, const FRotator& Rot = FRotator::ZeroRotator,
		float ShapeRadius = 5.0f, float HalfHeight = 0.0f, bool bDebugVisual = false
		);

	

	UFUNCTION(BlueprintCallable)
	void ReleaseGrabbedActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace Channel")
	TEnumAsByte<ECollisionChannel> TraceChannel;

	UPROPERTY(BlueprintAssignable)
	FOnGrabSignature OnGrabDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnFreezeSignature OnFreezeDelegate;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetTraceChannel(TEnumAsByte<ECollisionChannel> Channel) { TraceChannel = Channel; }

private:

	void InitializeHands();
	
};
