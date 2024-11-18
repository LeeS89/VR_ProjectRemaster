// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/GrabbableObject.h"
#include "TestGrabActor.generated.h"

UCLASS()
class PROJECTREMASTER_API ATestGrabActor : public AActor, public IGrabbableObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestGrabActor();

	virtual void OnGrabbed(FName SocketName) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
