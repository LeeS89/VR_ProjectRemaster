// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Interfaces/DamageableInterface.h>
#include "DamageHandler.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
    FOnDamageSignature,
    UDamageHandler, OnDamageDelegate,
    const float, DamageAmount
);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTREMASTER_API UDamageHandler : public UActorComponent
{
	GENERATED_BODY()

private:

    float DamagePerTick;
    float DoTDurationValue;
    FTimerHandle DoTTimerHandle;
    float RemainingDoTTicks;

    
    UPROPERTY(VisibleAnywhere)
    const class UElementalDamageType* ElementDamageTypeClass{ nullptr };

    UFUNCTION(BlueprintCallable, Category = "Damage", meta = (AllowPrivateAccess = "true"))
    void ApplyInstantDamage(float DamageAmount);

    UFUNCTION(BlueprintCallable, Category = "Damage", meta = (AllowPrivateAccess = "true"))
    void ApplyDoTEffect();

    void HandleElementalDamage(const class UElementalDamageType* DamageType, AController* EventInstigator, AActor* DamageCauser);

    void HandleDoT();

    void ClearDoTEffect();

    UFUNCTION(BlueprintCallable, Category = "Damage")
    void ApplyDamageOverTime();


public:	
	// Sets default values for this component's properties
	UDamageHandler();

    IDamageableInterface* DamageInterface;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void HandleDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

   
    UFUNCTION()
    void SetElementalEffectClass(const class UElementalDamageType* ElementDamageType/*, float InDamageOverTime, float InDoTDuration*/);
  
    UPROPERTY(BlueprintAssignable)
    FOnDamageSignature OnDamageDelegate;
};
