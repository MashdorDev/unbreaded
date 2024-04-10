// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DynamicCameraInterface.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "CookieInterface.h"
#include "GameplayAbilitySpec.h"
#include "GameplayEffectTypes.h"
#include "InteractInterface.h"
#include "SCharacter.generated.h"

class ASExplodingBody;
class UDynamicCameraComponent;
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;
class USGameplayAbility;
class UGameplayEffect;
class UAbilitySystemComponent;
class ASWeapon;
class USInteractionComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGamePauseInput);


USTRUCT(BlueprintType)
struct FCameraOccludedActor
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	const AActor* Actor;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMesh;
  
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UMaterialInterface*> Materials;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsOccluded;
};


UCLASS()
class UNBREAD_API ASCharacter : public ACharacter, public IDynamicCameraInterface, public IAbilitySystemInterface, public IInteractInterface, public ICookieInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UDynamicCameraComponent*  DynamicCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USInteractionComponent* InteractionComponent;
	
	APlayerCameraManager*  camMan;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category = "Input")
	FVector LookTargetPos;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* DefaultInputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* RotateAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;

	// GAMEPLAY ABILITY SYSTEM
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	UInputAction* PrimaryAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	UInputAction* SecondaryAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	UInputAction* MovementAbilityAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	UInputAction* InteractionAbilityAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	UInputAction* UtilityAbilityAction;

	// Weapon System

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapons", meta = (AllowPrivateAccess = "true"))
	ASWeapon* Weapon;

	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
	TSubclassOf<ASWeapon> DefaultWeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Occlusion|Materials")
	UMaterialInterface* FadeMaterial;

	// FUNCTIONS AND VARIABLES

	void Move(const FInputActionValue& Value);
	void Rotate(const FInputActionValue& Value);

	// Camera Rotation Speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite)//, Category = "Camera")
	float CameraRotationMultiplier = 2.0f;

	// JUMP

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GravityAppliedOnWalk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GravityAppliedOnFall;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsJumping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bJumpBuffered;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpBufferDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsLevelSequencePlaying;
	
	FTimerHandle JumpBufferTimer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RollAngleMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RollAngleMax;
	
	void Jump(const FInputActionValue& Value);
	void StopJumping() override;

	UFUNCTION(BlueprintCallable)
	void BufferJump();
	
	UFUNCTION(BlueprintCallable)
	void UnBufferJump();
	
	float Speed;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float BodySpeed;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float HeadSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float LerpSpeed = 0.6f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bUseNewRotation = true;

	// Splitting Variables
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsHeadForm = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* NearestCrumbles = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ASExplodingBody*> ActiveBodies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<APawn> BodyClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* CharacterMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* HeadMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HeadLaunchVelocityMultiplier = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HeadLaunchVelocityZAxisAdd = 1200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<const UStaticMeshComponent*, FCameraOccludedActor> OccludedActors;
	
	// GAS setup
	void OnPrimaryAttack(const FInputActionValue& Value);
	void OnSecondaryAttack(const FInputActionValue& Value);
	void OnMovementAbility(const FInputActionValue& Value);
	void OnInteractionAbility(const FInputActionValue& Value);
	void OnUtilityAbility(const FInputActionValue& Value);

	virtual void SendAbilityLocalInput(const FInputActionValue& Value, int32 InputID);

	UPROPERTY()
	APlayerController* PlayerController;


	void SetNextCamera_Implementation(AActor* CameraActor) override;

	void TransitionCamera_Implementation(const float TransitionTime) override;

	// Splitting Mechanic Functions 

	void SetNearestCrumblePile_Implementation(AActor* CrumblesActor) override;

	UFUNCTION(BlueprintCallable)
	void LaunchHead();
	
	UFUNCTION(BlueprintCallable)
	void LaunchHeadVertical();

	UFUNCTION(BlueprintCallable)
	void DestroyBodyAndSpawnCrumbles();

	UFUNCTION(BlueprintCallable)
	void ReformBody();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LaunchHeadMaxDuration;
	
	FTimerHandle LaunchHeadTimerHandle;

	void ResetLaunchHeadTimer();

	// MELEE INTERACT
	/*UFUNCTION(BlueprintImplementableEvent)
	void MeleeInteract();*/

	/*UFUNCTION(BlueprintImplementableEvent)
	void Punch();*/


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void HideOccludedActor(UStaticMeshComponent* Mesh);

	UFUNCTION(BlueprintCallable)
	void ForceShowActors();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Inherited via IAbilitySystemInterface
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void InitAbilitySystemComponent();

	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FGamePauseInput PauseGame;

protected:
	UPROPERTY()
	TWeakObjectPtr<class UAbilitySystemComponent> AbilitySystemComponent;

	virtual void InitializeAbilities();
	virtual void InitializeEffects();
	virtual void ClearGivenAbilities();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TArray<TSubclassOf<USGameplayAbility>> DefaultAbilities;
	TArray<FGameplayAbilitySpecHandle> GivenAbilites;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TArray<TSubclassOf<UGameplayEffect>> DefaultEffects;

	virtual void OnHealthAttributeChanged(const FOnAttributeChangeData& Data);

	UFUNCTION(BlueprintImplementableEvent, Category = "GAS")
	void OnHealthChanged(float OldValue, float NewValue);

	virtual void OnShieldAttributeChanged(const FOnAttributeChangeData& Data);

	UFUNCTION(BlueprintImplementableEvent, Category = "GAS")
	void OnShieldChanged(float OldValue, float NewValue);

	UFUNCTION(BlueprintCallable)
	void EquipWeapon(ASWeapon* NewWeapon);
};