// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

public:	
	// Sets default values for this component's properties
	UDamageHandler();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void HandleDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

    UFUNCTION(BlueprintCallable, Category = "Damage")
    void ApplyDamageOverTime();

    // Set DoT Amount
    UFUNCTION(BlueprintCallable, Category = "Damage")
    void SetDoT(float InDamageOverTime);

    // Set DoT Duration
    UFUNCTION(BlueprintCallable, Category = "Damage")
    void SetDoTDuration(float InDoTDuration);

    // Apply Instant Damage
    UFUNCTION(BlueprintCallable, Category = "Damage")
    void ApplyInstantDamage(float DamageAmount);

    UFUNCTION(BlueprintCallable, Category = "Damage")
    void ApplyDoTEffect();

    void HandleFireDamage(float DamageAmount, AController* EventInstigator, AActor* DamageCauser);

    void HandleDoT();

    UPROPERTY(BlueprintAssignable)
    FOnDamageSignature OnDamageDelegate;
};
