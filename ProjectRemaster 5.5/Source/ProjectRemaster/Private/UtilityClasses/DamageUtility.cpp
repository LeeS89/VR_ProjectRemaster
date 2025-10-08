// Fill out your copyright notice in the Description page of Project Settings.

#include "UtilityClasses/DamageUtility.h"
#include "Characters/MainCharacter.h"


/// <summary>
/// This function is used to determine if the result of a "hit" from a damage causer
/// is because of accidental friendly fire.
/// If both the instigator and Target Implement the same interface, meaning they are on the same team
/// then Damage should NOT be caused to the target
/// </summary>
/// <param name="Instigator"></param>
/// <param name="Target"></param>
/// <returns></returns>
bool UDamageUtility::ShouldDamageActor(const APawn* Instigator, AActor* Target)
{
    if (!Instigator || !Target)
    {
        return false;
    }

    UClass* InstigatorClass = Instigator->GetClass();
 
    
    for (const FImplementedInterface& Interface : InstigatorClass->Interfaces)
    {
        if (Interface.Class && Interface.Class->IsChildOf<UInterface>())
        {
            //UE_LOG(LogTemp, Warning, TEXT("Interface from array: %s"), *Interface.Class->GetName());
            if (Target->GetClass()->ImplementsInterface(Interface.Class))
            {
                //UE_LOG(LogTemp, Warning, TEXT("Target implements interface: %s"), *Interface.Class->GetName());
                return false; // Prevent damage
            }
        }
    }
    
    return true; 
}

bool UDamageUtility::DoesChanceOfEffectSucceed(int32 ChancePercentage)
{
    ChancePercentage = FMath::Clamp(ChancePercentage, 0, 100);
    UE_LOG(LogTemp, Error, TEXT("Chance of effect %i"), ChancePercentage);

    int32 RandomRoll = FMath::RandRange(1, 100);
    UE_LOG(LogTemp, Error, TEXT("Random Roll %i"), RandomRoll);
    return RandomRoll <= ChancePercentage;
    //return FMath::RandRange(1, 100) <= ChancePercentage;;
}
