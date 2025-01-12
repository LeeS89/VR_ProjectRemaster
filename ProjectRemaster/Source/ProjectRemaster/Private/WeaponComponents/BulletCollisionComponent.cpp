// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponents/BulletCollisionComponent.h"
#include "Components/StaticMeshComponent.h"
#include <UtilityClasses/DamageUtility.h>
#include "Engine/DamageEvents.h"
#include <Interfaces/DeflectableInterface.h>
#include <CapsuleComponent.h>
#include <Structs/FDamageTypeInfo.h>
#include <Interfaces/ElementalDamageInterface.h>

// Sets default values for this component's properties
UBulletCollisionComponent::UBulletCollisionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UBulletCollisionComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerRef = GetOwner();
	//UE_LOG(LogTemp, Error, TEXT("Calling Initialize Damage type"));
	
	if (!OwnerRef) { return; }

	MeshComp = OwnerRef->FindComponentByClass<UStaticMeshComponent>();

	if (!MeshComp) { return; }

	
	if (!OwnerRef->Implements<UDeflectableInterface>()) { return; }

	BulletInterface = Cast<IDeflectableInterface>(OwnerRef);

	InitializeDamageType(DamageType);

}


// Called every frame
void UBulletCollisionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBulletCollisionComponent::HandleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!ShouldRespondToHit(OtherActor, OtherComp)) { return; }

	BulletInterface->SetHitHasBeenProcessed(true);

	APawn* InstigatorPawn = OwnerRef->GetInstigator();
	if (!InstigatorPawn) { return; } // Need to modify here for situations where Instigstor is destroyed before a hit

	if (UDamageUtility::ShouldDamageActor(InstigatorPawn, OtherActor))
	{
		DamageTarget(OtherActor);
	}

	OnHitDelegate.Broadcast(true, Hit.ImpactPoint, Hit.ImpactPoint.Rotation());
	BulletInterface->OnExpired();

}

bool UBulletCollisionComponent::ShouldRespondToHit(AActor* OtherActor, UPrimitiveComponent* OtherComp)
{
	if (!OtherActor || OtherActor == GetOwner()) { return false; }
	if (BulletInterface->GetHitHasBeenProcessed() || !BulletInterface) { return false; }
	/*ECollisionResponse Response = OtherComp->GetCollisionResponseToChannel(MeshComp->GetCollisionObjectType());
	if (MeshComp && Response == ECollisionResponse::ECR_Ignore)
	{
		UE_LOG(LogTemp, Warning, TEXT("Collision response ignored for %s"), *OtherActor->GetName());
		return false;
	}*/

	return true;
}

void UBulletCollisionComponent::DamageTarget(AActor* ActorToDamage)
{
	if (!BulletInterface) { return; }

	FDamageEvent TargetDamageEvent;
	TargetDamageEvent.DamageTypeClass = DamageTypeClass;
	//UE_LOG(LogTemp, Error, TEXT("Damage class type and damage amount: %s, %f"), *DamageTypeClass->GetName(), Damage);
	//float Damage{ 0.0f };
	//Damage = BulletInterface->GetDamage();
	
	/*if (ActorToDamage->Implements<UDamageableInterface>())
	{
		IDamageableInterface* DamageInterface = Cast<IDamageableInterface>(ActorToDamage);
		DamageInterface->Execute_SetDoT(ActorToDamage, DamageOverTime);
		DamageInterface->Execute_SetDoTDuration(ActorToDamage, DoTDuration);
	}*/

	ActorToDamage->TakeDamage(
		Damage,
		TargetDamageEvent,
		GetOwner()->GetInstigatorController(),
		GetOwner()
	);
}

void UBulletCollisionComponent::InitializeDamageType(EDamageType DamageTypeToInitialize)
{

	if (!DamageDataTable) { return; }

	static const FString ContextString(TEXT("Damage Lookup"));
	//FDamageTypeInfo* DamageInfo = DamageDataTable->FindRow<FDamageTypeInfo>(FName(*DamageTypeName), ContextString);
	FDamageTypeInfo* DamageInfo = DamageDataTable->FindRow<FDamageTypeInfo>(FName(*UEnum::GetValueAsString(DamageTypeToInitialize)), ContextString);

	if (DamageInfo)
	{
		Damage = DamageInfo->BaseDamage;

		DamageOverTime = DamageInfo->DamageOverTime;

		DoTDuration = DamageInfo->Duration;

		DamageTypeClass = DamageInfo->DamageTypeClass;

	}

	if (!GetOwner()->Implements<UElementalDamageInterface>()) { return; }

	IElementalDamageInterface* DamageInterface = Cast<IElementalDamageInterface>(GetOwner());
	if (DamageInterface)
	{
		DamageInterface->SetDoTAmount(DamageOverTime);
		DamageInterface->SetDoTDuration(DoTDuration);
	}

}

