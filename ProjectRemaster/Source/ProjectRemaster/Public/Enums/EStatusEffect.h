// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum EStatusEffect : uint8
{
	NoEffect	UMETA(DisplayName = "No Effect"),
	Burned		UMETA(DisplayName = "Burned"),
	Poisoned	UMETA(DisplayName = "Poisoned")
};
