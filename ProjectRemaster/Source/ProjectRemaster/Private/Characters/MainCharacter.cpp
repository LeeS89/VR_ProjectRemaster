// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "HandPoseRecognizer.h"
#include "CharacterComponents/CustomHandPoseRecognizer.h"
#include "CharacterComponents/CustomXRHandComponent.h"
#include "CharacterComponents/TraceComponent.h"
#include "CharacterComponents/VRCameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "CharacterComponents/StatsComponent.h"
#include "CharacterComponents/LocomotionComponent.h"
#include <Components/Image.h>
#include "CharacterComponents/DamageHandler.h"
#include "CharacterComponents/PlayerAbilitiesComponent.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	VROrigin = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	VROrigin->SetupAttachment(RootComponent);


	VRCameraComp = CreateDefaultSubobject<UVRCameraComponent>(TEXT("Camera Component"));
	VRCameraComp->SetupAttachment(RootComponent);

	BlackoutWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Blackout Widget Component"));
	BlackoutWidgetComp->SetupAttachment(VRCameraComp);

	///// Come back to later
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	BoxComp->SetupAttachment(VRCameraComp);

	DamageCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Collider Component"));
	DamageCollider->SetupAttachment(VRCameraComp);


	// Hand Components - Visuals, Gesture Recognizers and tracking
	LeftMotionControllerComp = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Left Motion Controller Component"));
	LeftMotionControllerComp->SetupAttachment(VROrigin);

	RightMotionControllerComp = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Right Motion Controller Component"));
	RightMotionControllerComp->SetupAttachment(VROrigin);

	LeftXRHandComp = CreateDefaultSubobject<UCustomXRHandComponent>(TEXT("Left Oculus XR Hand Component"));
	LeftXRHandComp->SetupAttachment(LeftMotionControllerComp);

	RightXRHandComp = CreateDefaultSubobject<UCustomXRHandComponent>(TEXT("Right Oculus XR Hand Component"));
	RightXRHandComp->SetupAttachment(RightMotionControllerComp);

	// Left Hand Pose Recognizers
	LeftPoseRecognizer = CreateDefaultSubobject<UHandPoseRecognizer>(TEXT("Left Hand Pose Recognizer"));
	LeftPoseRecognizer->SetupAttachment(LeftMotionControllerComp);


	MovePoseRecognizerLeft = CreateDefaultSubobject<UCustomHandPoseRecognizer>(TEXT("Fist Pose Recognizer Left"));
	MovePoseRecognizerLeft->SetupAttachment(LeftMotionControllerComp);


	// Right Hand Pose Recognizers
	RightPoseRecognizer = CreateDefaultSubobject<UHandPoseRecognizer>(TEXT("Right Hand Pose Recognizer"));
	RightPoseRecognizer->SetupAttachment(RightMotionControllerComp);


	MovePoseRecognizerRight = CreateDefaultSubobject<UCustomHandPoseRecognizer>(TEXT("Fist Pose Recognizer Right"));
	MovePoseRecognizerRight->SetupAttachment(RightMotionControllerComp);

	// Actor Components
	LocomotionComp = CreateDefaultSubobject<ULocomotionComponent>(TEXT("Locomotion Component"));

	TraceComp = CreateDefaultSubobject<UTraceComponent>(TEXT("Trace Component"));

	StatsComp = CreateDefaultSubobject<UStatsComponent>(TEXT("Stats Component"));

	//AbilitiesComp = CreateDefaultSubobject<UPlayerAbilitiesComponent>(TEXT("Abilities Component"));

	DamageHandlerComp = CreateDefaultSubobject<UDamageHandler>(TEXT("Damage Handler Component"));
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	
	/*if (BlackoutWidgetClass)
	{

		BlackoutWidget = CreateWidget<UUserWidget>(GetWorld(), BlackoutWidgetClass);

		BlackoutWidget->AddToViewport();
	}*/
	OnTakeAnyDamage.AddDynamic(this, &AMainCharacter::OnReceiveAnyDamage);

	

	if (BlackoutWidget)
	{
		BlackoutWidgetComp->SetWidgetClass(BlackoutWidget);

		BlackoutWidgetInstance = BlackoutWidgetComp->GetUserWidgetObject();
	}

	BoxComp->SetCollisionObjectType(ECC_GameTraceChannel3);
	//BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::OnOverlapBegin);

	
}


// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

USceneComponent* AMainCharacter::GetTargetComponent()
{
	return DamageCollider;
}


float AMainCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageHandlerComp->HandleDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	return DamageAmount;
}

void AMainCharacter::OnReceiveAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Error, TEXT("I got hurt"));
}

void AMainCharacter::SetElementalClass_Implementation(const class UElementalDamageType* ElementDamageType)
{
	if (!DamageHandlerComp) { return; }

	DamageHandlerComp->SetElementalEffectClass(ElementDamageType);
}

TEnumAsByte<EDamageType> AMainCharacter::GetCurrentStatus() const
{
	return StatsComp->StatusEffect;
}

void AMainCharacter::UpdateStatusEffect(EDamageType NewStatus)
{
	StatsComp->StatusEffect = NewStatus; 
}

//void AMainCharacter::GetTraceLocation(FVector& OutLocation, FQuat& OutRotation)
//{
//	OutLocation = DamageCollider->GetComponentLocation();
//	OutRotation = DamageCollider->GetComponentRotation().Quaternion();
//}


void AMainCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		

		if (BlackoutWidget)
		{
			
			UImage* BlackoutImage = Cast<UImage>(BlackoutWidgetInstance->GetWidgetFromName(TEXT("BlackoutImage")));
			if (BlackoutImage)
			{
				//FLinearColor NewColors = BlackoutImage->GetColorAndOpacity().GetSpecifiedColor();

				FLinearColor NewColor = BlackoutImage->GetColorAndOpacity();
				NewColor.A = 1.0f; // Ensure Opacity is within valid range
				BlackoutImage->SetColorAndOpacity(NewColor);
				//BlackoutImage->SetRenderOpacity(1.0f); // Fully visible
			}
			
		}
	}
}



