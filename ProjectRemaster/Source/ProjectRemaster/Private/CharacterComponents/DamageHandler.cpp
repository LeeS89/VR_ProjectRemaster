// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterComponents/DamageHandler.h"
#include "GameFramework/DamageType.h"
#include "Engine/DamageEvents.h"
#include "Enums/EDamageType.h"
#include "DamageTypes/ElementalDamageType.h"





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
	ApplyInstantDamage(DamageAmount);

	//if (DamageCauser->Implements<UElementalDamageInterface>())
	//{
	
		UElementalDamageType* DamageTypeInstance = DamageEvent.DamageTypeClass
			? Cast<UElementalDamageType>(DamageEvent.DamageTypeClass->GetDefaultObject())
			: nullptr;

		//if (!DamageTypeInterface || !DamageTypeInstance) { return; }

		HandleElementalDamage(DamageAmount, DamageTypeInstance, EventInstigator, DamageCauser);
	
	//}
	
}

void UDamageHandler::HandleElementalDamage(float DamageAmount, UElementalDamageType* DamageType, AController* EventInstigator, AActor* DamageCauser)
{
	if (!DamageType) { return; }
	//float DoTPerTick = DamageType->GetAmoutOfDoT();
	//float Duration = DamageType->GetDoTDuration();
	
	//SetDoTEffect(DoTPerTick, Duration);
	//if(DamageType->bCausedByWorld)
	switch (DamageType->GetElementType())
	{
	case EDamageType::Fire:
		ApplyDamageOverTime();
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("No Damage Type"));
		break;
	}
	//if (DamageType && DamageType->IsA(UFireDamageType::StaticClass()))
	//{
		// Will eventually play a particle effect here
	
	//}
	

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
/// this function gets called with values taken from the damage type class
/// </summary>
/// <param name="InDamageOverTime">Amount of damage per tick</param>
/// <param name="InDoTDuration">Number of ticks to apply damage for</param>
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





