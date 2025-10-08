// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum ETraceType : uint8
{
	NoTrace				UMETA(DisplayName = "None"),
	GrabTrace			UMETA(DisplayName = "Grab Trace"),
	BulletFreezeTrace	UMETA(DisplayName = "Bullet Freeze Trace")
};
