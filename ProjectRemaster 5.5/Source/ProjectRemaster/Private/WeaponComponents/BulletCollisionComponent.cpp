// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponents/BulletCollisionComponent.h"
#include "Components/StaticMeshComponent.h"
#include <UtilityClasses/DamageUtility.h>
#include <Interfaces/DeflectableInterface.h>
#include <CapsuleComponent.h>
#include <Structs/FDamageTypeInfo.h>
#include "DamageTypes/ElementalDamageType.h"
#include "Engine/DataTable.h"
#include <Kismet/GameplayStatics.h>


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
	
	if (!OwnerRef) { return; }

	MeshComp = OwnerRef->FindComponentByClass<UStaticMeshComponent>();

	if (!MeshComp) { return; }// Not sure if used anymore

	
	if (!OwnerRef->Implements<UDeflectableInterface>()) { return; }

	BulletInterface = Cast<IDeflectableInterface>(OwnerRef);

	if (!DamageDataTable || !BulletInterface) { return; }

	DamageType = BulletInterface->GetDamageType();
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
	
	return true;
}

void UBulletCollisionComponent::DamageTarget(AActor* ActorToDamage)
{
	if (!BulletInterface) { return; }

	OnSetDoTParamsDelegate.Broadcast(ActorToDamage, DamageTypeInstance/*, (DamageOverTime * DamageMultiplier), DoTDuration*/ );
	
	ActorToDamage->TakeDamage(
		Damage,
		TargetDamageEvent,
		GetOwner()->GetInstigatorController(),
		GetOwner()
	);
}

void UBulletCollisionComponent::InitializeDamageType(EDamageType DamageTypeToInitialize)
{

	static const FString ContextString(TEXT("Damage Lookup"));

	FDamageTypeInfo* DamageInfo = DamageDataTable->FindRow<FDamageTypeInfo>(FName(*UEnum::GetValueAsString(DamageTypeToInitialize)), ContextString);

	if (!DamageInfo) { return; }

	Damage = DamageInfo->BaseDamage;

	DamageOverTime = DamageInfo->DamageOverTime;
	
	DoTDuration = DamageInfo->Duration;

	DamageTypeClass = DamageInfo->DamageTypeClass;


	if (DamageTypeClass)
	{
		DamageTypeInstance = NewObject<UElementalDamageType>(this, DamageTypeClass);
		if (DamageTypeInstance)
		{
			DamageTypeInstance->StatusEffectChancePercentage = StatusEffectChancePercentage;
			DamageTypeInstance->SetDamageOverTimeAmount(DamageOverTime * DamageMultiplier);
			DamageTypeInstance->SetDoTDuration(DoTDuration);

			TargetDamageEvent.DamageTypeClass = DamageTypeInstance->GetClass();
		}
	}
}

