#pragma once

#include "CoreMinimal.h"

#include "Interfaces/PooledObjectInterface.h"

template <typename T>
class TObjectPool
{
private:
    int32 PoolSize;
    TSubclassOf<T> ObjectClass;
    UWorld* World;

    TArray<T*> ObjectPool;

public:
    TObjectPool(int32 InPoolSize, TSubclassOf<T> InObjectClass, UWorld* InWorld)
        : PoolSize(InPoolSize), ObjectClass(InObjectClass), World(InWorld)
    {
    }

    void InitializePool()
    {
        if (!ObjectClass || !World) return;

        for (int32 i = 0; i < PoolSize; ++i)
        {
            T* NewObject = World->SpawnActor<T>(ObjectClass, FVector::ZeroVector, FRotator::ZeroRotator);
            if (NewObject && NewObject->GetClass()->ImplementsInterface(UPooledObjectInterface::StaticClass()))
            {
                IPooledObjectInterface* Interface = Cast<IPooledObjectInterface>(NewObject);
                if (Interface)
                {
                    Interface->SetIsInUse(false);
                    Interface->ToggleActiveState(false);
                    ObjectPool.Add(NewObject);
                }
            }
        }
    }

    T* GetObjectFromPool()
    {
        for (T* Object : ObjectPool)
        {
            if (Object && Object->GetClass()->ImplementsInterface(UPooledObjectInterface::StaticClass()))
            {
                IPooledObjectInterface* Interface = Cast<IPooledObjectInterface>(Object);
                if (Interface && !Interface->IsInUse())
                {
                    Interface->SetIsInUse(true);
                    return Object;
                }
            }
        }
        return nullptr;
    }

    void ReturnObjectToPool(T* Object)
    {
        if (Object && Object->GetClass()->ImplementsInterface(UPooledObjectInterface::StaticClass()))
        {
            IPooledObjectInterface* Interface = Cast<IPooledObjectInterface>(Object);
            if (Interface)
            {
                //UE_LOG(LogTemp, Error, TEXT("BULLET SHOULD BE INVISIBLE NOW"));
                Interface->SetIsInUse(false);
                Interface->ToggleActiveState(false);
            }
        }
    }
};