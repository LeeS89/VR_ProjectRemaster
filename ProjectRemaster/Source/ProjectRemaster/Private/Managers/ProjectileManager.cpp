// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/ProjectileManager.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Projectiles/BaseBullet.h"


// Sets default values
AProjectileManager::AProjectileManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    //PrimaryActorTick.TickInterval = 0.2f;
    FireInstancedBulletMesh = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("Fire InstancedBulletMesh"));
    RootComponent = FireInstancedBulletMesh;

    PoisonInstancedBulletMesh = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("Poison InstancedBulletMesh"));
    PoisonInstancedBulletMesh->SetupAttachment(RootComponent);

    MergedBulletParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("MergedBulletParticles"));
    MergedBulletParticles->SetupAttachment(RootComponent);

    FireInstancedBulletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    FireInstancedBulletMesh->SetCastShadow(false);

    PoisonInstancedBulletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    PoisonInstancedBulletMesh->SetCastShadow(false);
}



// Called when the game starts or when spawned
void AProjectileManager::BeginPlay()
{
	Super::BeginPlay();
    
    if (BulletEffectSystem)
    {
        MergedBulletParticles->SetAsset(BulletEffectSystem);
        MergedBulletParticles->Activate();
    }

}

void AProjectileManager::AddFrozenBullet(TEnumAsByte<EDamageType> DamageType, ABaseBullet* Bullet, UStaticMeshComponent* BulletMesh)
{
    if (DamageType == EDamageType::NoType || !Bullet || !BulletMesh || FrozenBullets.Contains(Bullet)) return;

    FrozenBullets.Add(Bullet);
    CreateBulletInstance(DamageType, Bullet, BulletMesh);
   
    /*
    if (FireInstancedBulletMesh->GetStaticMesh() == nullptr)
    {
        FireInstancedBulletMesh->SetStaticMesh(BulletMesh->GetStaticMesh());
        FireInstancedBulletMesh->SetMaterial(0, BulletMesh->GetMaterial(0));
    }

    FTransform BulletWorldTransform = Bullet->GetActorTransform(); 
    FTransform BulletLocalTransform = BulletWorldTransform.GetRelativeTransform(FireInstancedBulletMesh->GetComponentTransform());

    int32 InstanceIndex = FireInstancedBulletMesh->AddInstance(BulletLocalTransform);
    
    UpdateNiagaraParticles(FireInstancedBulletMesh, InstanceIndex);
    */
}

void AProjectileManager::CreateBulletInstance(TEnumAsByte<EDamageType> DamageType, ABaseBullet* Bullet, UStaticMeshComponent* BulletMesh)
{
    switch (DamageType)
    {
    case EDamageType::Fire:
        TargetMesh = FireInstancedBulletMesh;
        break;
    default:
        UE_LOG(LogTemp, Warning, TEXT("No Compatible Mesh Found"));
        break;
    }

    if (!TargetMesh) { return; }

    if (TargetMesh->GetStaticMesh() == nullptr)
    {
        TargetMesh->SetStaticMesh(BulletMesh->GetStaticMesh());
        TargetMesh->SetMaterial(0, BulletMesh->GetMaterial(0));
    }

    FTransform BulletWorldTransform = Bullet->GetActorTransform();
    FTransform BulletLocalTransform = BulletWorldTransform.GetRelativeTransform(TargetMesh->GetComponentTransform());

    int32 InstanceIndex = TargetMesh->AddInstance(BulletLocalTransform);

    UpdateNiagaraParticles(DamageType, TargetMesh, InstanceIndex);
}

void AProjectileManager::RemoveFrozenBullet(ABaseBullet* Bullet)
{
    if (!Bullet || !FireInstancedBulletMesh || !MergedBulletParticles) { return; }

    int32 InstanceIndex = FrozenBullets.Find(Bullet);
    
    if (InstanceIndex == INDEX_NONE) { return; }

   
    FTransform InstanceTransform;
    FireInstancedBulletMesh->GetInstanceTransform(InstanceIndex, InstanceTransform, true);
    FVector BulletPos = InstanceTransform.GetLocation();


    FrozenBullets.RemoveAt(InstanceIndex);
    
    RemoveFrozenBulletParticles_BP(BulletPos);
    FireInstancedBulletMesh->RemoveInstance(InstanceIndex);
    if (FireInstancedBulletMesh->GetInstanceCount() <= 0)
    {
        //MergedBulletParticles->DestroyInstance();
        MergedBulletParticles->Deactivate();
    }
}




void AProjectileManager::UpdateNiagaraParticles(TEnumAsByte<EDamageType> DamageType, UHierarchicalInstancedStaticMeshComponent* TMPMesh, int32 InstanceIndex)
{
    if (!MergedBulletParticles || !FireInstancedBulletMesh) return;

    FTransform InstanceTransform;
    if (TMPMesh->GetInstanceTransform(InstanceIndex, InstanceTransform, true))
    {
        FVector BulletLocation = InstanceTransform.GetLocation();   
        // Call Blueprint function with the full array
        UpdateFrozenBulletParticles_BP(DamageType, BulletLocation, InstanceIndex);

        TargetMesh = nullptr;
    }
}




#pragma region Redundant Code
void AProjectileManager::UpdateInstanceCulling()
{
    if (!FireInstancedBulletMesh) return;

    FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();

    //for (int32 i = 0; i < InstancedBulletMesh->GetInstanceCount(); i++)
    //{


        /*if (!OriginalInstanceTransforms.Contains(i))
        {
            FTransform CurrentTransform;
            InstancedBulletMesh->GetInstanceTransform(i, CurrentTransform, true);
            OriginalInstanceTransforms.Add(i, CurrentTransform);
        }*/
        //FTransform InstanceTransform = OriginalInstanceTransforms[i];

        //float DistanceToPlayer = FVector::Dist(PlayerLocation, InstanceTransform.GetLocation());

    //    if (DistanceToPlayer < 30.0f) // Too close, hide
    //    {
    //        InstanceTransform.SetScale3D(FVector(0.0f));  // Scale down instance


    //        //if (InstanceToParticleMap.Contains(i))
    //        //{
    //        //    UParticleSystemComponent* Particle = InstanceToParticleMap[i];
    //        //    if (Particle)
    //        //    {
    //        //        Particle->SetVisibility(false);  // Hide particle
    //        //    }
    //        //}
    //        InstancedBulletMesh->UpdateInstanceTransform(i, InstanceTransform, true, true, true);
    //    }
    //    else // Restore visibility
    //    {
    //        InstanceTransform.SetScale3D(FVector(1.0f));  // Restore instance size


    //        //if (InstanceToParticleMap.Contains(i))
    //        //{
    //        //    UParticleSystemComponent* Particle = InstanceToParticleMap[i];
    //        //    if (Particle)
    //        //    {
    //        //        Particle->SetVisibility(true);  // Show particle
    //        //    }
    //        //}
    //        InstancedBulletMesh->UpdateInstanceTransform(i, InstanceTransform, true, true, true);
    //    }

    //}


}

void AProjectileManager::StartCullingTimer()
{
    //if (!GetWorld()->GetTimerManager().IsTimerActive(CullCheckTimerHandle))
    //{
    //    GetWorld()->GetTimerManager().SetTimer(
    //        CullCheckTimerHandle,
    //        this,
    //        &AProjectileManager::UpdateInstanceCulling, // Function to run
    //        0.2f,   // Interval 
    //        true    // Looping
    //    );
    //}
    //else
    //{
    //    return;
    //}
}

#pragma endregion

