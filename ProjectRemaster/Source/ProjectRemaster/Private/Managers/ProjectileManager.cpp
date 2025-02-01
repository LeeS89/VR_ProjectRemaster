// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/ProjectileManager.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Projectiles/BaseBullet.h"


// Sets default values
AProjectileManager::AProjectileManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    //PrimaryActorTick.TickInterval = 0.2f;
    InstancedBulletMesh = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("InstancedBulletMesh"));
    RootComponent = InstancedBulletMesh;

    
    InstancedBulletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    InstancedBulletMesh->SetCastShadow(false);
}

// Called when the game starts or when spawned
void AProjectileManager::BeginPlay()
{
	Super::BeginPlay();
    
}

void AProjectileManager::AddFrozenBullet(ABaseBullet* Bullet, UStaticMeshComponent* BulletMesh, UParticleSystemComponent* BulletParticles)
{
    
    if (!Bullet || FrozenBullets.Contains(Bullet)) return;

    FrozenBullets.Add(Bullet);

    // Get bullet mesh
    if (!BulletMesh) return;

    StartCullingTimer();


    // Set the same mesh on the Instanced Static Mesh Component if it's not set
    if (InstancedBulletMesh->GetStaticMesh() == nullptr)
    {
        InstancedBulletMesh->SetStaticMesh(BulletMesh->GetStaticMesh());
        InstancedBulletMesh->SetMaterial(0, BulletMesh->GetMaterial(0));
    }

    // Get world transform of the bullet
    FTransform BulletWorldTransform = Bullet->GetActorTransform();

    // Convert world transform to relative transform (relative to the InstancedBulletMesh)
    FTransform BulletLocalTransform = BulletWorldTransform.GetRelativeTransform(InstancedBulletMesh->GetComponentTransform());

    int32 InstanceIndex = InstancedBulletMesh->AddInstance(BulletLocalTransform);
    
    //InstanceToBulletMap.Add(InstanceIndex, Bullet);
    InstanceToParticleMap.Add(InstanceIndex, BulletParticles);
    //Bullet->SetActorHiddenInGame(true);
    Bullet->SetActorEnableCollision(false);
    BulletMesh->SetVisibility(false);
    
}

void AProjectileManager::StartCullingTimer()
{
    if (!GetWorld()->GetTimerManager().IsTimerActive(CullCheckTimerHandle))
    {
        GetWorld()->GetTimerManager().SetTimer(
            CullCheckTimerHandle,
            this,
            &AProjectileManager::UpdateInstanceCulling, // Function to run
            0.2f,   // Interval 
            true    // Looping
        );
    }
}

void AProjectileManager::UpdateInstanceCulling()
{
    if (!InstancedBulletMesh) return;

    FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();

    for (int32 i = 0; i < InstancedBulletMesh->GetInstanceCount(); i++)
    {
       
        
        if (!OriginalInstanceTransforms.Contains(i))
        {
            FTransform CurrentTransform;
            InstancedBulletMesh->GetInstanceTransform(i, CurrentTransform, true);
            OriginalInstanceTransforms.Add(i, CurrentTransform);
        }
        FTransform InstanceTransform = OriginalInstanceTransforms[i];

        float DistanceToPlayer = FVector::Dist(PlayerLocation, InstanceTransform.GetLocation());

        if (DistanceToPlayer < 30.0f) // Too close, hide
        {
            InstanceTransform.SetScale3D(FVector(0.0f));  // Scale down instance
            

            if (InstanceToParticleMap.Contains(i))
            {
                UParticleSystemComponent* Particle = InstanceToParticleMap[i];
                if (Particle)
                {
                    Particle->SetVisibility(false);  // Hide particle
                }
            }
            InstancedBulletMesh->UpdateInstanceTransform(i, InstanceTransform, true, true, true);
        }
        else // Restore visibility
        {
            InstanceTransform.SetScale3D(FVector(1.0f));  // Restore instance size
           

            if (InstanceToParticleMap.Contains(i))
            {
                UParticleSystemComponent* Particle = InstanceToParticleMap[i];
                if (Particle)
                {
                    Particle->SetVisibility(true);  // Show particle
                }
            }
            InstancedBulletMesh->UpdateInstanceTransform(i, InstanceTransform, true, true, true);
        }
        
    }

    
}