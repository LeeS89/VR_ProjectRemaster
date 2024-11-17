// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum EHand : uint8
{
	None UMETA(DisplayName = "None Selected"),
	Left UMETA(DisplayName = "Left Hand"),
	Right UMETA(DisplayName = "Right Hand")
};
