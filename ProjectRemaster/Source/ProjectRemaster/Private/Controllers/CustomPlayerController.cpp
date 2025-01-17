// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/CustomPlayerController.h"

ACustomPlayerController::ACustomPlayerController()
{

}


void ACustomPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle DoTTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		DoTTimerHandle,
		this,
		&ACustomPlayerController::TestTimer,
		5.0f,
		false
	);
}

void ACustomPlayerController::TestTimer()
{
	EnableInput(this);
}

void ACustomPlayerController::EnablePlayerInput()
{
	EnableInput(this);
}

void ACustomPlayerController::DisablePlayerInput()
{
	DisableInput(this);
}

void ACustomPlayerController::DisableInput(APlayerController* PlayerController)
{
	Super::DisableInput(PlayerController);
	OnInputDisabled.Broadcast();
}

void ACustomPlayerController::EnableInput(APlayerController* PlayerController)
{
	Super::EnableInput(PlayerController);
	OnInputEnabled.Broadcast();
}
