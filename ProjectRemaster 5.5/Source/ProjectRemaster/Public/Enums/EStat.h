// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum EStat
{
	NoStat UMETA(DisplayName = "None Selected"),
	Health UMETA(DisplayName = "Health"),
	MaxHealth UMETA(DisplayName = "Max Health"),
	Force UMETA(DisplayName = "Force"),
	MaxForce UMETA(DisplayName = "Max Force")
};
