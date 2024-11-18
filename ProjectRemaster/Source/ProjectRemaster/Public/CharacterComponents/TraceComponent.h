// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TraceComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTREMASTER_API UTraceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTraceComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	//UPROPERTY(VisibleAnywhere)
	class UCustomXRHandComponent* LeftHandComp;

	//UPROPERTY(VisibleAnywhere)
	class UCustomXRHandComponent* RightHandComp;

	UPROPERTY(VisibleAnywhere)
	USkeletalMesh* LeftHandMesh;

	UPROPERTY(VisibleAnywhere)
	USkeletalMesh* RightHandMesh;

	class IMainPlayer* IPlayerRef;
};
