// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponents/BulletCollisionComponent.h"
#include "Components/StaticMeshComponent.h"
#include <UtilityClasses/DamageUtility.h>
#include "Engine/DamageEvents.h"
#include <Interfaces/DeflectableInterface.h>
#include <CapsuleComponent.h>

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

	if (!MeshComp) { return; }

	MeshComp->OnComponentHit.AddDynamic(this, &UBulletCollisionComponent::HandleHit);
	/*if (MeshComp->OnComponentHit.IsBound())
	{
		UE_LOG(LogTemp, Error, TEXT("HANDLEHIT IS BOUND YAAAAY"));
	}*/
	if (!OwnerRef->Implements<UDeflectableInterface>()) { return; }

	BulletInterface = Cast<IDeflectableInterface>(OwnerRef);

	//UE_LOG(LogTemp, Error, TEXT("Collision Enabled: %d"), MeshComp->GetCollisionEnabled());
	//UE_LOG(LogTemp, Error, TEXT("Generate Hit Events: %d"), MeshComp->GetGenerateOverlapEvents());

	
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
	if (!InstigatorPawn) { return; }

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
	ECollisionResponse Response = OtherComp->GetCollisionResponseToChannel(MeshComp->GetCollisionObjectType());
	if (MeshComp && Response == ECollisionResponse::ECR_Ignore)
	{
		UE_LOG(LogTemp, Warning, TEXT("Collision response ignored for %s"), *OtherActor->GetName());
		return false;
	}

	return true;
}

void UBulletCollisionComponent::DamageTarget(AActor* ActorToDamage)
{
	FDamageEvent TargetAttackedEvent;
	float Damage{ 0.0f };

	if (BulletInterface)
	{
		Damage = BulletInterface->GetDamage();

		ActorToDamage->TakeDamage(
			Damage,
			TargetAttackedEvent,
			GetOwner()->GetInstigatorController(),
			GetOwner()
		);
	}
}

