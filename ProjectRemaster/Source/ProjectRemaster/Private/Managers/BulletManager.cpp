// Fill out your copyright notice in the Description page of Project Settings.

#include "Managers/BulletManager.h"
#include "Projectiles/BaseBullet.h"
#include "InstancedStaticMeshComponent.h"
#include "ProceduralMeshComponent.h"




// Sets default values
ABulletManager::ABulletManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    /*MergedVertices.Empty();
    MergedTriangles.Empty();
    ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("FrozenBulletMesh"));
    ProceduralMesh->SetupAttachment(RootComponent);
    ProceduralMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    ProceduralMesh->SetSimulatePhysics(false);
    ProceduralMesh->SetVisibility(true);
    ProceduralMesh->bUseComplexAsSimpleCollision = false;*/
    InstancedBulletMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedBulletMesh"));
    InstancedBulletMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ABulletManager::BeginPlay()
{
	Super::BeginPlay();
	
}




void ABulletManager::AddFrozenBullet(ABaseBullet* Bullet, UStaticMeshComponent* BulletMesh, UParticleSystemComponent* BulletParticles)
{
    if (!Bullet || FrozenBullets.Contains(Bullet)) return;

    FrozenBullets.Add(Bullet);

    // Get bullet mesh
    //UStaticMeshComponent* BulletMesh = Bullet->FindComponentByClass<UStaticMeshComponent>();
    if (!BulletMesh) return;

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

    // Add bullet instance to instanced mesh
    InstancedBulletMesh->AddInstance(BulletLocalTransform);
    Bullet->SetActorEnableCollision(false);
    //BulletMesh->SetVisibility(false);
    Bullet->SetActorHiddenInGame(true);
    // Destroy the original bullet actor (or just hide it if needed)
    //Bullet->Destroy();
}











void ABulletManager::HandleFrozenBulletMerge(ABaseBullet* Bullet)
{

    if (!Bullet) return;

   
    // Disable individual bullet
    Bullet->SetActorEnableCollision(false);
    Bullet->SetActorTickEnabled(false);

    /*if (UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(Bullet->GetRootComponent()))
    {
        MeshComp->SetVisibility(false);
    }*/

   
    // Store bullet reference and transform
    //FrozenBullets.Add(Bullet);
    FrozenTransforms.Add(Bullet->GetActorTransform());

    // Merge bullet into procedural mesh
    MergeBulletIntoFrozenMesh(Bullet);
}

void ABulletManager::MergeBulletIntoFrozenMesh(ABaseBullet* Bullet)
{
    if (!Bullet || !ProceduralMesh || FrozenBullets.Contains(Bullet))
        return; // Already merged, skip.

    FrozenBullets.Add(Bullet);
    BulletsToMerge.Add(Bullet); // Add to buffer

    //Bullet->SetActorHiddenInGame(true);
    Bullet->SetActorEnableCollision(false);

    // Start a timer to process merging periodically
    if (!GetWorld()->GetTimerManager().IsTimerActive(MergeTimerHandle))
    {
        GetWorld()->GetTimerManager().SetTimer(
            MergeTimerHandle, this, &ABulletManager::ProcessBulletMerging, 0.1f, true
        );
    }
    /*if (!Bullet || !ProceduralMesh) return;

    UStaticMeshComponent* BulletMesh = Cast<UStaticMeshComponent>(Bullet->GetRootComponent());
    if (!BulletMesh) return;

    UStaticMesh* Mesh = BulletMesh->GetStaticMesh();
    if (!Mesh || !Mesh->GetRenderData()) return;

    FTransform BulletTransform = Bullet->GetActorTransform();
    FStaticMeshRenderData* RenderData = Mesh->GetRenderData();

    //// Use LOD 0 (highest detail) for merging
    FStaticMeshLODResources& LODResources = RenderData->LODResources[0];
    FPositionVertexBuffer& VertexBuffer = LODResources.VertexBuffers.PositionVertexBuffer;
    FIndexArrayView IndexBuffer = LODResources.IndexBuffer.GetArrayView();

    TArray<FVector> Vertices;
    TArray<int32> Indices;

    for (uint32 v = 0; v < VertexBuffer.GetNumVertices(); v++)
    {
        //Vertices.Add(BulletTransform.TransformPosition(FVector(VertexBuffer.VertexPosition(v))));
        Vertices.Add(ProceduralMesh->GetComponentTransform().InverseTransformPosition(
           BulletTransform.TransformPosition(FVector(VertexBuffer.VertexPosition(v)))
        ));
        //Vertices.Add(FVector(VertexBuffer.VertexPosition(v)));
    }

    for (int32 idx = 0; idx < IndexBuffer.Num(); idx++)
    {
        Indices.Add(IndexBuffer[idx]);
    }
    /////////////////////////
    ////ProceduralMesh->CreateMeshSection(0, Vertices, Indices, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), true);
    ////int32 SectionIndex = FrozenBullets.Num() - 1; // Each new bullet gets a new section
    ////ProceduralMesh->CreateMeshSection(SectionIndex, Vertices, Indices, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), true);
    //////////////////////////
    //
    int32 VertexOffset = MergedVertices.Num();
    for (int32& Index : Indices)
    {
        Index += VertexOffset;
    }

    //// Append new bullet's vertex & triangle data
    MergedVertices.Append(Vertices);
    MergedTriangles.Append(Indices);

    //// Recreate the **entire** procedural mesh section with the accumulated data
    ProceduralMesh->CreateMeshSection(
        0,  // Always modify the first section
        MergedVertices,
        MergedTriangles,
        TArray<FVector>(),
        TArray<FVector2D>(),
        TArray<FColor>(),
        TArray<FProcMeshTangent>(),
        true
    );



    ProceduralMesh->SetMaterial(0, BulletMesh->GetMaterial(0));*/
}



void ABulletManager::ProcessBulletMerging()
{
    if (BulletsToMerge.Num() == 0)
    {
        // Stop the timer if there are no bullets to process
        GetWorld()->GetTimerManager().ClearTimer(MergeTimerHandle);
        return;
    }

    TArray<FVector> NewVertices;
    TArray<int32> NewIndices;

    for (ABaseBullet* Bullet : BulletsToMerge)
    {
        UStaticMeshComponent* BulletMesh = Cast<UStaticMeshComponent>(Bullet->GetRootComponent());
        if (!BulletMesh) continue;
        BulletMesh->SetVisibility(false);
        UStaticMesh* Mesh = BulletMesh->GetStaticMesh();
        if (!Mesh || !Mesh->GetRenderData()) continue;

        FTransform BulletTransform = Bullet->GetActorTransform();
        FStaticMeshRenderData* RenderData = Mesh->GetRenderData();

        FStaticMeshLODResources& LODResources = RenderData->LODResources[0];
        FPositionVertexBuffer& VertexBuffer = LODResources.VertexBuffers.PositionVertexBuffer;
        FIndexArrayView IndexBuffer = LODResources.IndexBuffer.GetArrayView();

        TArray<FVector> BulletVertices;
        TArray<int32> BulletIndices;

        // Extract vertex positions
        for (uint32 v = 0; v < VertexBuffer.GetNumVertices(); v++)
        {
            BulletVertices.Add(ProceduralMesh->GetComponentTransform().InverseTransformPosition(
                BulletTransform.TransformPosition(FVector(VertexBuffer.VertexPosition(v)))
            ));
        }

        // Extract index data
        for (int32 idx = 0; idx < IndexBuffer.Num(); idx++)
        {
            BulletIndices.Add(IndexBuffer[idx]);
        }

        if (BulletVertices.Num() == 0 || BulletIndices.Num() == 0) continue;

        // Offset indices
        int32 VertexOffset = MergedVertices.Num();
        for (int32& Index : BulletIndices)
        {
            Index += VertexOffset;
        }

        // Append new bullet's mesh data
        MergedVertices.Append(BulletVertices);
        MergedTriangles.Append(BulletIndices);
    }

    // Recreate **only section 0** instead of adding multiple sections
    ProceduralMesh->CreateMeshSection(
        0,
        MergedVertices,
        MergedTriangles,
        TArray<FVector>(),
        TArray<FVector2D>(),
        TArray<FColor>(),
        TArray<FProcMeshTangent>(),
        true
    );

    // Clear the buffer after merging
    BulletsToMerge.Empty();
}


