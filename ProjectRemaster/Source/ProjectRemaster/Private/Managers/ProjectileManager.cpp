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
    FireInstancedBulletMesh->SetupAttachment(RootComponent);
    
    //RootComponent = FireInstancedBulletMesh;

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

#pragma region Bullet Freezing

void AProjectileManager::AddFrozenBullet(TEnumAsByte<EDamageType> DamageType, ABaseBullet* Bullet, UStaticMeshComponent* BulletMesh)
{
    if (DamageType == EDamageType::NoType || !Bullet || !BulletMesh || AllFrozenBullets.Contains(Bullet)) return;

    if (AllFrozenBullets.Num() <= 0)
    {
        MergedBulletParticles->Activate();
    }

    AllFrozenBullets.Add(Bullet);
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
    TargetMesh = nullptr;

    switch (DamageType)
    {
    case EDamageType::Fire:
        //TargetMesh = FireInstancedBulletMesh;
        FrozenFireBullets.Add(Bullet);
        UpdateInstanceMesh(DamageType, Bullet, BulletMesh, FireInstancedBulletMesh);
        
        break;
    case EDamageType::Poison:
        //TargetMesh = PoisonInstancedBulletMesh;
        UpdateInstanceMesh(DamageType, Bullet, BulletMesh, PoisonInstancedBulletMesh);
        FrozenPoisonBullets.Add(Bullet);
        break;
    default:
        UE_LOG(LogTemp, Warning, TEXT("No Compatible Mesh Found"));
        break;
    }

    //if //(!TargetMesh) { return; }
    //UpdateInstaceMesh(DamageType, Bullet, BulletMesh, TargetMesh);

    /*if (TargetMesh->GetStaticMesh() == nullptr)
    {
        TargetMesh->SetStaticMesh(BulletMesh->GetStaticMesh());
        TargetMesh->SetMaterial(0, BulletMesh->GetMaterial(0));
    }

    FTransform BulletWorldTransform = Bullet->GetActorTransform();
    FTransform BulletLocalTransform = BulletWorldTransform.GetRelativeTransform(TargetMesh->GetComponentTransform());

    int32 InstanceIndex = TargetMesh->AddInstance(BulletLocalTransform);

    UpdateNiagaraParticles(DamageType, TargetMesh, InstanceIndex);*/
}




void AProjectileManager::UpdateInstanceMesh(TEnumAsByte<EDamageType> DamageType, ABaseBullet* Bullet, UStaticMeshComponent* BulletMesh, UHierarchicalInstancedStaticMeshComponent* InstanceMesh)
{
    if (!Bullet || !BulletMesh || !InstanceMesh) { return; }

    if (InstanceMesh->GetStaticMesh() == nullptr)
    {
        InstanceMesh->SetStaticMesh(BulletMesh->GetStaticMesh());
        InstanceMesh->SetMaterial(0, BulletMesh->GetMaterial(0));
    }

    FTransform BulletWorldTransform = Bullet->GetActorTransform();
    FTransform BulletLocalTransform = BulletWorldTransform.GetRelativeTransform(InstanceMesh->GetComponentTransform());

    int32 InstanceIndex = InstanceMesh->AddInstance(BulletLocalTransform);
    UE_LOG(LogTemp, Error, TEXT("%s BulletsCount: %i"),*InstanceMesh->GetName(), InstanceMesh->GetInstanceCount());
    /*switch (DamageType)
    {
    case EDamageType::Fire:
        FireBulletIndices.Add(Bullet, InstanceIndex);
        break;
    case EDamageType::Poison:
        PoisonBulletIndices.Add(Bullet, InstanceIndex);
        break;
    default:
        UE_LOG(LogTemp, Warning, TEXT("No Bullet Type Found"));
        break;
    }*/

    UpdateNiagaraParticles(DamageType, InstanceMesh, InstanceIndex);
}
#pragma endregion

void AProjectileManager::RemoveFrozenBullet(TEnumAsByte<EDamageType> DamageType, ABaseBullet* Bullet)
{
    if (!Bullet) return;

    UHierarchicalInstancedStaticMeshComponent* TempMesh = nullptr;

    int32 FrozenIndex = AllFrozenBullets.Find(Bullet);
    if (FrozenIndex == INDEX_NONE) { return; }

    int32 ElementIndex = 0;

    switch (DamageType)
    {
    case EDamageType::Fire:
        TempMesh = FireInstancedBulletMesh;
        ElementIndex = FrozenFireBullets.Find(Bullet);
        FrozenFireBullets.Remove(Bullet);
        break;
    case EDamageType::Poison:
        TempMesh = PoisonInstancedBulletMesh;
        ElementIndex = FrozenPoisonBullets.Find(Bullet);
        FrozenPoisonBullets.Remove(Bullet);
        break;
    default:
        break;
    }

    if (!TempMesh || ElementIndex == INDEX_NONE) { return; }

    FTransform InstanceTransform;
    TempMesh->GetInstanceTransform(ElementIndex, InstanceTransform, true);
    FVector BulletPos = InstanceTransform.GetLocation();

    AllFrozenBullets.RemoveAt(FrozenIndex);

    RemoveFrozenBulletParticles_BP(DamageType, BulletPos);
    TempMesh->RemoveInstance(ElementIndex);


    if (FireInstancedBulletMesh->GetInstanceCount() <= 0 &&
        PoisonInstancedBulletMesh->GetInstanceCount() <= 0)
    {
        MergedBulletParticles->Deactivate();
        MergedBulletParticles->SetNiagaraVariableFloat(TEXT("user.FireKillRadius"), 0.0f);
        MergedBulletParticles->SetNiagaraVariableFloat(TEXT("user.PoisonKillRadius"), 0.0f);
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
        UpdateFrozenBulletParticles_BP(DamageType, BulletLocation);

        //TargetMesh = nullptr;
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

