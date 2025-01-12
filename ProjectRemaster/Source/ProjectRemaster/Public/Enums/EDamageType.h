// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum EDamageType : uint8
{
    Fire      UMETA(DisplayName = "Fire"),
    Ice       UMETA(DisplayName = "Ice"),
    Electric  UMETA(DisplayName = "Electric"),
    Water     UMETA(DisplayName = "Water")
};