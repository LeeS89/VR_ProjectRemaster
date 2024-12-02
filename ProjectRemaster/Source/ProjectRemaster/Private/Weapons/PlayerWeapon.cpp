// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/PlayerWeapon.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CharacterComponents/CustomXRHandComponent.h"

// Sets default values
APlayerWeapon::APlayerWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = RootComp;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	StaticMeshComp->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void APlayerWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void APlayerWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsGrabbed) { return; }


	FVector StartSocketLocation{ StaticMeshComp->GetSocketLocation(StartSocket) };
	FVector EndSocketLocation{ StaticMeshComp->GetSocketLocation(EndSocket) };
	FQuat SocketRotation{ StaticMeshComp->GetSocketQuaternion(UseSocketRotation) };

	

	FCollisionShape Capsule{
		FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight)
	};

	FCollisionQueryParams IgnoreParams{
		FName{TEXT("Ignore Params")},
		true,
		this
	};


	FHitResult OutResult;

	bool bHasFoundTarget{ GetWorld()->SweepSingleByChannel(
		OutResult,
		StartSocketLocation,
		EndSocketLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		Capsule,
		IgnoreParams
	) };


	///////////////////////////
	// Trace Hit Logic Here
	///////////////////////////

	FVector CenterPoint{
		UKismetMathLibrary::VLerp(
			StartSocketLocation, EndSocketLocation, 0.5f
)
	};

	UKismetSystemLibrary::DrawDebugCapsule(
		GetWorld(),
		CenterPoint,
		Capsule.GetCapsuleHalfHeight(),
		Capsule.GetCapsuleRadius(),
		SocketRotation.Rotator(),
		bHasFoundTarget ? FLinearColor::Green : FLinearColor::Red,
		0.1f,
		1.0f
	);

	

}

void APlayerWeapon::OnGrabbed_Implementation(UCustomXRHandComponent* HandComponent, FName SocketName)
{
	bIsGrabbed = true;
	OnWeaponGrabbedDelegate.Broadcast();
	AttachToComponent(HandComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
}

void APlayerWeapon::OnReleased_Implementation()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	OnWeaponReleasedDelegate.Broadcast();
	bIsGrabbed = false;
}

bool APlayerWeapon::IsGrabbed()
{
	return bIsGrabbed;
}

