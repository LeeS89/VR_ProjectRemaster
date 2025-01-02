#pragma once

#include "CoreMinimal.h"

#include "Interfaces/PooledObjectInterface.h"

template <typename T>
class TObjectPool
{
private:
    TArray<T*> ObjectPool;
    TSubclassOf<T> ObjectClass;
    int32 PoolSize;
    UWorld* World;

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
                Interface->SetIsInUse(false);
                Interface->ToggleActiveState(false);
            }
        }
    }
};