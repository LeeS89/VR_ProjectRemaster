// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/MainPlayer.h"
#include "GameFramework/Character.h"
#include "Interfaces/DamageableInterface.h"
#include "Interfaces/ResettableComponent.h"
#include "MainCharacter.generated.h"

UCLASS()
class PROJECTREMASTER_API AMainCharacter : public ACharacter, public IMainPlayer, public IDamageableInterface
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere)
	float PlayerCapsuleMinHeight{ 50.0f };

	UPROPERTY(EditAnywhere)
	float PlayerCapsuleMaxHeight{ 180.0f };

	class UUserWidget* BlackoutWidgetInstance;

	class ACustomPlayerController* PC;
	//TArray<IResettableComponent*> ResettableComponents;
	UPROPERTY(VisibleAnywhere)
	TArray<TScriptInterface<IResettableComponent>> ResettableComponents;

public:
	// Sets default values for this character's properties
	AMainCharacter();

	// Camera and Hand Visuals
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UVRCameraComponent* VRCameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* BoxComp;

	// Hand Components - Visuals, Gesture Recognizers and tracking
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UMotionControllerComponent* LeftMotionControllerComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UMotionControllerComponent* RightMotionControllerComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UCustomXRHandComponent* LeftXRHandComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UCustomXRHandComponent* RightXRHandComp;


	// Hand Pose Recognizers
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UHandPoseRecognizer* LeftPoseRecognizer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UHandPoseRecognizer* RightPoseRecognizer;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UCustomHandPoseRecognizer* MovePoseRecognizerLeft;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UCustomHandPoseRecognizer* MovePoseRecognizerRight;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USceneComponent* VROrigin;


	// Actor Components
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Locomotion")
	class ULocomotionComponent* LocomotionComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tracing")
	class UTraceComponent* TraceComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	class UPlayerAbilitiesComponent* AbilitiesComp;

	// Out of bounds widget
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "UI")
	class UWidgetComponent* BlackoutWidgetComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class UUserWidget> BlackoutWidget;

	//Damage Collider, stats component
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UBoxComponent* DamageCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStatsComponent* StatsComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UDamageHandler* DamageHandlerComp;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY(BlueprintReadOnly)
	UCapsuleComponent* PlayerCapsuleComp;


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual USceneComponent* GetTargetComponent() override;

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

	UFUNCTION()
	virtual void OnReceiveAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	virtual void SetElementalClass_Implementation(const class UElementalDamageType* ElementDamageType) override;

	UFUNCTION()
	virtual TEnumAsByte<EDamageType> GetCurrentStatus() const override;

	UFUNCTION(BlueprintCallable)
	virtual	void UpdateStatusEffect(EDamageType NewStatus) override;

	UFUNCTION()
	virtual bool CheckFrozenBulletcountGreaterThanZero() override;

	UFUNCTION()
	virtual void HandleInputDisabled() override;

	

private:

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						bool bFromSweep, const FHitResult& SweepResult);

	void RegisterResettableComponents();

	UFUNCTION()
	void HandleComponentDisabling();
};
