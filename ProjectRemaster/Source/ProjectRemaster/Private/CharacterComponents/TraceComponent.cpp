// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/Character.h"
#include "Interfaces/MainPlayer.h"
#include "Enums/EHand.h"
#include "CharacterComponents/CustomXRHandComponent.h"
#include "Components/SkeletalMeshComponent.h"
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

	TArray<UCustomXRHandComponent*> HandComponents = IPlayerRef->GetHandComponents();

	for (UCustomXRHandComponent* HandComponent : HandComponents)
	{
		if (HandComponent)
		{
			EHand CurrentHandType = HandComponent->GrabHandSide;

			if (CurrentHandType == EHand::Left)
			{
				LeftHandMesh = HandComponent->SkeletalMesh;
			}
			else if (CurrentHandType == EHand::Right)
			{
				RightHandMesh = HandComponent->SkeletalMesh;
			}
		}
	}
	
}


// Called every frame
void UTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

