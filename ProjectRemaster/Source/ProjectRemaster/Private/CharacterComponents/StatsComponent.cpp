// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterComponents/StatsComponent.h"
#include <Kismet/KismetMathLibrary.h>

// Sets default values for this component's properties
UStatsComponent::UStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	StatusEffect = EDamageType::NoType;
	//StatusEffect = EStatusEffect::Poisoned;
	// ...
	
}


// Called every frame
void UStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UStatsComponent::ReduceHealth(float Amount)
{
	Stats[EStat::Health] -= Amount;

	Stats[EStat::Health] = UKismetMathLibrary::FClamp(Stats[EStat::Health], 0.0f, Stats[EStat::MaxHealth]);

	/////////
	// Delegate Here to broadcast health updated for widget
	/////////

	if (Stats[EStat::Health] == 0)
	{
		OnZeroHealthDelegate.Broadcast();
	}
}


