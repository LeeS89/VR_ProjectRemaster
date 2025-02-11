// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum EHandInControl : uint8
{
	Neither UMETA(DisplayName = "None"),
	LeftHand UMETA(DisplayName = "Left Hand Control"),
	RightHand UMETA(DisplayName = "Right Hand Control")
};
