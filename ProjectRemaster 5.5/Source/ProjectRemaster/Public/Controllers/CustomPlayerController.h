// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CustomPlayerController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInputEnabled);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInputDisabled);
/**
 * 
 */
UCLASS()
class PROJECTREMASTER_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	ACustomPlayerController();

	UPROPERTY(BlueprintAssignable, Category = "Input")
	FOnInputEnabled OnInputEnabled;

	UPROPERTY(BlueprintAssignable, Category = "Input")
	FOnInputDisabled OnInputDisabled;

	UFUNCTION()
	void TestTimer();

	UFUNCTION()
	void EnablePlayerInput();

	UFUNCTION()
	void DisablePlayerInput();

protected:

	virtual void BeginPlay() override;

	virtual void DisableInput(APlayerController* PlayerController) override;

	virtual void EnableInput(APlayerController* PlayerController) override;

	
};
