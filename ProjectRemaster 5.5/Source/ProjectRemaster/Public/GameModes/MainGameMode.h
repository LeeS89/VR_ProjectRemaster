// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameMode.generated.h"


class ACharacter;
/**
 * 
 */
UCLASS()
class PROJECTREMASTER_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:

	ACharacter* PlayerCharacter;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable)
	ACharacter* GetPlayerCharacter();

	UFUNCTION(BlueprintCallable)
	void PawnKilled(APawn* PawnKilled);

	virtual void StartPlay() override;

	virtual void RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot) override;
	
};
