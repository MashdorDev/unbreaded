// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DynamicCameraInterface.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpec.h"
#include "InteractInterface.h"
#include "SCharacter.generated.h"

class UDynamicCameraComponent;
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;
class USGameplayAbility;
class UGameplayEffect;

UCLASS()
class UNBREAD_API ASCharacter : public ACharacter, public IDynamicCameraInterface, public IAbilitySystemInterface, public IInteractInterface
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

	// TEMP
	// TODO: review after full merge with camera system

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UDynamicCameraComponent*  DynamicCamera;

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

	// TEMPORARY!!!
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	UInputAction* ProjectileAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	UInputAction* SprintAction;

	// FUNCTIONS AND VARIABLES

	void Move(const FInputActionValue& Value);
	//void Rotate(const FInputActionValue& Value);
	void RotateToTarget(const FVector LookAtTarget);

	// TEMPORARY CHARACTER SETUP

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsJumping;

	UPROPERTY(EditAnywhere)
	int JumpCount;

	void CheckJump();

	void Jump(const FInputActionValue& Value);

	float Speed;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float WalkSpeed;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float SprintSpeed;

	UPROPERTY(EditAnywhere, Category = "Movement")
	bool bIsWalking;

	void Sprint();

	// TEMPORARY PROJECTILE ATTACK
	UPROPERTY(EditAnywhere)
    TSubclassOf<AActor> ProjectileClass;

	void CheckAmmo();
	void ShootProjectile();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	int MaxAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	int CurrentAmmo;

	// TODO: UPDATE TEMPORARY SETUP USING GAS


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


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Inherited via IAbilitySystemInterface
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void InitAbilitySystemComponent();

	virtual void PossessedBy(AController* NewController) override;

	virtual void Landed(const FHitResult& Hit) override;

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
};