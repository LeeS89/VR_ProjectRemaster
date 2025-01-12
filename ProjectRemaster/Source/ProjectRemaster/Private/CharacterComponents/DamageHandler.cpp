// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterComponents/DamageHandler.h"
#include "GameFramework/DamageType.h"
#include "Engine/DamageEvents.h"
#include "DamageTypes/FireDamageType.h"
#include <Interfaces/ElementalDamageInterface.h>


// Sets default values for this component's properties
UDamageHandler::UDamageHandler()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UDamageHandler::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDamageHandler::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDamageHandler::HandleDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageCauser->Implements<UElementalDamageInterface>())
	{
		IElementalDamageInterface* DamageInterface = Cast<IElementalDamageInterface>(DamageCauser);
		const UDamageType* DamageTypeInstance = DamageEvent.DamageTypeClass ? DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>() : nullptr;
		
		if (!DamageInterface || !DamageTypeInstance) { return; }

		float DoTPerTick = DamageInterface->GetDoTAmount();
		float Duration = DamageInterface->GetDoTDuration();

		SetDoTEffect(DoTPerTick, Duration);

		if (DamageEvent.DamageTypeClass == UFireDamageType::StaticClass())
		{
			HandleFireDamage(DamageAmount, EventInstigator, DamageCauser);
		}
	}
	else
	{
		ApplyInstantDamage(DamageAmount);
	}
}

void UDamageHandler::HandleFireDamage(float DamageAmount, AController* EventInstigator, AActor* DamageCauser)
{
	ApplyInstantDamage(DamageAmount);
	ApplyDamageOverTime();
}

void UDamageHandler::ApplyInstantDamage(float DamageAmount)
{
	OnDamageDelegate.Broadcast(DamageAmount);
}

/// <summary>
/// Damage received from elemental sources eg. fire, poison etc..
/// Actor receives continuous damage for a short time
/// </summary>
void UDamageHandler::ApplyDamageOverTime()
{
	//DamagePerTick = InDamagePerTick;
	RemainingDoTTicks = DoTDurationValue;

	GetWorld()->GetTimerManager().SetTimer(
		DoTTimerHandle,
		this,
		&UDamageHandler::HandleDoT,
		1.0f,
		true
	);
}

void UDamageHandler::SetDoTEffect(float InDamageOverTime, float InDoTDuration)
{
	DamagePerTick = InDamageOverTime;
	DoTDurationValue = InDoTDuration;
}

void UDamageHandler::HandleDoT()
{
	if (--RemainingDoTTicks > 0)
	{
		ApplyInstantDamage(DamagePerTick);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(DoTTimerHandle);
		DamagePerTick = 0.0f;
		DoTDurationValue = 0.0f;
	}
}


void UDamageHandler::ApplyDoTEffect()
{
}





