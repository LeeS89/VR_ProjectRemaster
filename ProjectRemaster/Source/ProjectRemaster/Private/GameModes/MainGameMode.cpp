// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/MainGameMode.h"
#include "GameFramework/Character.h"
#include <Kismet/GameplayStatics.h>
#include <Controllers/CustomPlayerController.h>

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();

	/*PlayerCharacter = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (PlayerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("PLAYER FOUND!!!!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PLAYER NOT FOUND!!!!"));
	}*/
}

ACharacter* AMainGameMode::GetPlayerCharacter()
{
	if (!PlayerCharacter)
	{
		PlayerCharacter = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}
	return PlayerCharacter;
}


void AMainGameMode::StartPlay()
{
	Super::StartPlay();

	PlayerCharacter = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

}


void AMainGameMode::PawnKilled(APawn* PawnKilled)
{
	if (!PawnKilled) { return; }

	AController* Controller = PawnKilled->GetController();

	if (!Controller) { return; }
	if (ACustomPlayerController* PC = Cast<ACustomPlayerController>(Controller))
	{
		PC->DisablePlayerInput();
		UE_LOG(LogTemp, Error, TEXT("Player Has been Killed!!!!!"));
	}
}