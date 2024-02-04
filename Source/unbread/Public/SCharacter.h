// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DynamicCameraInterface.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpec.h"
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
class UNBREAD_API ASCharacter : public ACharacter, public IDynamicCameraInterface, public IAbilitySystemInterface
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
<<<<<<< HEAD
=======
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;
>>>>>>> 3617c70 (lol)

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;

<<<<<<< HEAD
	

=======
>>>>>>> 3617c70 (lol)
	// TEMP
	// TODO: review after full merge with camera system
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ForwardDirectionIndicatorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UDynamicCameraComponent*  DynamicCamera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

<<<<<<< HEAD
	UPROPERTY(BlueprintReadOnly, Category = "Input")
	FVector LookTargetPos;

=======
>>>>>>> 3617c70 (lol)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* DefaultInputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* RotateAction;
<<<<<<< HEAD
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;
=======
>>>>>>> 3617c70 (lol)

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

<<<<<<< HEAD
	// TEMPORARY!!!
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	UInputAction* ProjectileAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = true))
	UInputAction* SprintAction;

	// FUNCTIONS AND VARIABLES
	
=======
	// FUNCTIONS AND VARIABLES

>>>>>>> 3617c70 (lol)
	void Move(const FInputActionValue& Value);
	//void Rotate(const FInputActionValue& Value);
	void RotateToTarget(const FVector LookAtTarget);

<<<<<<< HEAD
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

	UPROPERTY(EditAnywhere)
    TSubclassOf<AActor> ProjectileClass;
	
	void ShootProjectile();

	// TODO: UPDATE TEMPORARY SETUP USING GAS

=======
>>>>>>> 3617c70 (lol)
	// GAS setup
	void OnPrimaryAttack(const FInputActionValue& Value);
	void OnSecondaryAttack(const FInputActionValue& Value);
	void OnMovementAbility(const FInputActionValue& Value);
	void OnInteractionAbility(const FInputActionValue& Value);
	void OnUtilityAbility(const FInputActionValue& Value);
	
	virtual void SendAbilityLocalInput(const FInputActionValue& Value, int32 InputID);

<<<<<<< HEAD
=======
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MoveSpeed = 100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float RotationRate = 100.f;

>>>>>>> 3617c70 (lol)
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

<<<<<<< HEAD
	virtual void Landed(const FHitResult& Hit) override;

=======
>>>>>>> 3617c70 (lol)
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
<<<<<<< HEAD

	virtual void OnShieldAttributeChanged(const FOnAttributeChangeData& Data);

	UFUNCTION(BlueprintImplementableEvent, Category = "GAS")
	void OnShieldChanged(float OldValue, float NewValue);
=======
>>>>>>> 3617c70 (lol)
};
