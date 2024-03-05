// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCharacter.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISightTargetInterface.h"
#include "unbread/unbread.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpec.h"

#include "SRangedAICharacter.generated.h"

class UAbilitySystemComponent;
class USWeaponAttributeSet;
class USHealthAttributeSet;
class USGameplayAbility;
class UGameplayEffect;

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
class UNBREAD_API ASRangedAICharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = true))
	UAbilitySystemComponent* AbilitySystemComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = true))
	USHealthAttributeSet* HealthAttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = true))
	USWeaponAttributeSet* WeaponAttributeSet;

public:
	// Sets default values for this character's properties
	ASRangedAICharacter();

	// *** GAS *** //
	
	// Inherited from IAbilitySystemInterface
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void InitializeAbilities();
	virtual void InitializeEffects();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
	TArray<TSubclassOf<USGameplayAbility>> DefaultAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS")
	TArray<TSubclassOf<UGameplayEffect>> DefaultEffects;

	virtual void OnHealthAttributeChanged(const FOnAttributeChangeData& Data);

	UFUNCTION(BlueprintImplementableEvent, Category = "GAS")
	void OnHealthChanged(float OldValue, float NewValue);

	virtual void OnShieldAttributeChanged(const FOnAttributeChangeData& Data);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "GAS")
	void OnShieldChanged(float OldValue, float NewValue);

	virtual void OnDamageTakenChanged(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayTagContainer& GameplayTagContainer, float Damage);

	UFUNCTION(BlueprintImplementableEvent, Category = "GAS")
	void OnDamageTaken(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayTagContainer& GameplayTagContainer, float Damage);

	virtual void PostInitializeComponents() override;
	
	// ** --
	
	UPROPERTY(BlueprintReadOnly)
	class ASRanged_AIController* ControllerRef = nullptr;
	
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "AI")
	float FireRate = 0.1f;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "AI")
	ECombatRole CombatRole;
	
	
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

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "AI")
	float BaseDamage = 0.01f;
	
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

	UFUNCTION()
	FHitResult CapsuleTrace();

	UFUNCTION(BlueprintCallable)
	void StartWaponFire();

	UFUNCTION(BlueprintCallable)
	void StopWeaponFire();

	UFUNCTION(BlueprintCallable)
	void MakeSound(FVector Location);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
	FTimerHandle FireHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile")
	TSubclassOf<class ASProjectile> Projectile;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
	float Health = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PartickleEmitter")
	UParticleSystem* BloodFX;
	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess="true"))
	UBehaviorTree* Tree;
};
