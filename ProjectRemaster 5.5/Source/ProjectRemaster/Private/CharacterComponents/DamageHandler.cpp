// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterComponents/DamageHandler.h"
#include "GameFramework/DamageType.h"
#include "Engine/DamageEvents.h"
#include "Enums/EDamageType.h"
#include "DamageTypes/ElementalDamageType.h"
#include <UtilityClasses/DamageUtility.h>



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

	if (!GetOwner()->Implements<UDamageableInterface>()) { return; }

	DamageInterface = Cast<IDamageableInterface>(GetOwner());
	
}


// Called every frame
void UDamageHandler::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDamageHandler::HandleDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	ApplyInstantDamage(DamageAmount);

	if (!ElementDamageTypeClass) { return; }
	HandleElementalDamage(ElementDamageTypeClass, EventInstigator, DamageCauser);
}

/// <summary>
/// If actor currently has a Buff effect, function exits immediately
/// Otherwise, there will be a random chance of a buff effect being applied to the actor
/// Based on the percentage chance of the buff being applied from the retrieved damage type class
/// We then update the actors stats component through owners interface to update the current status effect
/// The the damage over time params are retrieved and set and then DoT is appled
/// And based on the type of elemental damage, we broadcaast this for the VFX component to play particles
/// </summary>
/// <param name="DamageType"></param>
/// <param name="EventInstigator"></param>
/// <param name="DamageCauser"></param>
void UDamageHandler::HandleElementalDamage(const UElementalDamageType* DamageType, AController* EventInstigator, AActor* DamageCauser)
{
	if (!DamageType || !DamageInterface) { return; }
	
	if (DamageInterface->GetCurrentStatus() != EDamageType::NoType) { return; }
	if (!UDamageUtility::DoesChanceOfEffectSucceed(DamageType->StatusEffectChancePercentage))
	{
		return;
	}
	DamageInterface->UpdateStatusEffect(DamageType->GetElementType());
	
	DamagePerTick = DamageType->GetDamageOverTimeAmount();
	DoTDurationValue = DamageType->GetDoTDuration();

	switch (DamageType->GetElementType())
	{
	case EDamageType::Fire:

		ApplyDamageOverTime();
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("No Damage Type"));
		break;
	}
	
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

/// <summary>
/// When the actor receives damage from elemental sources
/// this function gets called through the Owners interface to set the damage type class
/// from which we can then retrieve damage over time and dot duration values
/// </summary>
/// <param name="InDamageOverTime">Amount of damage per tick</param>
/// <param name="InDoTDuration">Number of ticks to apply damage for</param>
void UDamageHandler::SetElementalEffectClass(const UElementalDamageType* ElementDamageType/*, float InDamageOverTime, float InDoTDuration*/ )
{
	//if (ElementDamageTypeClass) { return; } // Come back to if decide to allow stacked effects
	ElementDamageTypeClass = ElementDamageType;
	
}

void UDamageHandler::HandleDoT()
{
	if (--RemainingDoTTicks > 0)
	{
		ApplyInstantDamage(DamagePerTick);
	}
	else
	{
		ClearDoTEffect();
	}
}

void UDamageHandler::ClearDoTEffect()
{
	GetWorld()->GetTimerManager().ClearTimer(DoTTimerHandle);
	DamagePerTick = 0.0f;
	DoTDurationValue = 0.0f;
	DamageInterface->UpdateStatusEffect(EDamageType::NoType);
	if (ElementDamageTypeClass)
	{
		ElementDamageTypeClass = nullptr;
	}
}


void UDamageHandler::ApplyDoTEffect()
{
}





