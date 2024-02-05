// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCharacter.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISightTargetInterface.h"
#include "unbread/unbread.h"
#include "SRangedAICharacter.generated.h"

USTRUCT(BlueprintType)
struct FAnimValues
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category = Movement)
	bool bIsCrouching = false;

	UPROPERTY(BlueprintReadWrite, Category = Movement)
	bool bIsInCombat = false;

	UPROPERTY(BlueprintReadWrite, Category = Movement)
	bool bIsShooting = false;

	UPROPERTY(BlueprintReadWrite, Category = Movement)
	bool bADS = false;

	UPROPERTY(BlueprintReadWrite, Category = Movement)
	bool bIsSitting = false;

};


UCLASS()
class UNBREAD_API ASRangedAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASRangedAICharacter();

	UPROPERTY(BlueprintReadOnly)
	class ASRanged_AIController* ControllerRef = nullptr;

	virtual bool CanBeSeenFrom
(
	const FVector & ObserverLocation,
	FVector & OutSeenLocation,
	int32 & NumberOfLoSChecksPerformed,
	float & OutSightStrength,
	const AActor * IgnoreActor,
	const bool * bWasVisible,
	int32 * UserData
) const;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UBehaviorTree* GetBehaviourTree() const;
	void ToggleCombat(bool Newbool);
	void ToggleCrouch(bool Newbool);
	void ToggleADS(bool Newbool);
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	FAnimValues AnimValues;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	class AS_SmartObject* SmartObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	bool Dead = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed = 110.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float CrouchedWalkSpeed = 100.f;


	
	// might change this
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	FName PerceptionTarget = "Spine";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	EFaction faction;

	// might change this
	bool IsHostile(ASCharacter* character);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateWindgerRef();
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateWidgetVis(bool NewBool);

	UFUNCTION(BlueprintCallable)
	void MakeSound(FVector Location);
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PartickleEmitter")
	UParticleSystem* BloodFX;
	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess="true"))
	UBehaviorTree* Tree;
};
