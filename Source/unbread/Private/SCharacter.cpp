// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "unbread/DynamicCameraComponent.h"
#include "../unbread.h"
#include "AbilitySystemComponent.h"
#include "SPlayerState.h"


// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(34.f, 88.f);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetMesh());

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	BaseMesh->SetupAttachment(GetMesh());
	
	ForwardDirectionIndicatorMesh = CreateDefaultSubobject<UStaticMeshComponent>("ForwardDirectionIndicatorMesh");
	ForwardDirectionIndicatorMesh->SetupAttachment(BaseMesh);

	DynamicCamera = CreateDefaultSubobject<UDynamicCameraComponent>("DynamicCamera");
	
	// TODO: ADD PROJECTILE SPAWN POINT

}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Add Enhanced Input Mapping Context
	PlayerController = Cast<APlayerController>(GetController());
	if(PlayerController)
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultInputMappingContext, 0);
		}
	}
}

void ASCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MoveVector = Value.Get<FVector2D>();
	
	// Forward / Backward
	const FVector Forward = FVector(1.f, 0.f,0.f);
	AddMovementInput(Forward, MoveVector.Y * MoveSpeed);
	

	// Right / Left
	const FVector Right = FVector(0.f, 1.f,0.f);
	AddMovementInput(Right, MoveVector.X * MoveSpeed);

	// TODO: Update forward and right vectors according to camera position and rotation
	//
	//
}

/*void ASCharacter::Rotate(const FInputActionValue& Value)
{
	
}*/

void ASCharacter::RotateToTarget(const FVector LookAtTarget)
{

	// METHOD 1
	
	//const FRotator Rotation = Controller->GetControlRotation();
	//const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	//FRotator DeltaRotation = FRotator::ZeroRotator;
	// FRotator DeltaRotation = Value.Get<FRotator>();
	// DeltaRotation.Yaw *= RotationRate * UGameplayStatics::GetWorldDeltaSeconds(this);
	// AddActorLocalRotation(DeltaRotation, true);

	// METHOD 2
	
	/*const FVector2D RotateAxisValue = Value.Get<FVector2D>();
	if(GetController())
	{
		AddControllerYawInput(RotateAxisValue.X);
		AddControllerPitchInput(RotateAxisValue.Y);
	}	*/

	// METHOD 3

	FVector ToTarget = LookAtTarget - ForwardDirectionIndicatorMesh->GetComponentLocation(); // this is a world rotation
	FRotator LookAtRotation(0.f, ToTarget.Rotation().Yaw, 0.f); //

	BaseMesh->SetWorldRotation(FMath::RInterpTo(BaseMesh->GetComponentRotation(),LookAtRotation, UGameplayStatics::GetWorldDeltaSeconds(this), 10.f));

	// TODO: Update rotation according to camera, lerp as tank
}


void ASCharacter::SetNextCamera_Implementation(AActor* CameraActor)
{
	IDynamicCameraInterface::SetNextCamera_Implementation(CameraActor);
	if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Set Next Camera Called"));
	DynamicCamera->SetNextCamera(CameraActor);
}

void ASCharacter::TransitionCamera_Implementation(const float TransitionTime)
{
	IDynamicCameraInterface::TransitionCamera_Implementation(TransitionTime);
	DynamicCamera->TransitionCamera(TransitionTime);
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Using APlayerController::GetHitResultUnderCursor to line trace to mouse cursor and getting hit information
	if(PlayerController)
	{
		// We're passing the FHitResult as reference but not const, because we need to change the information on HitResult with every hit.
		FHitResult HitResult;
		PlayerController->GetHitResultUnderCursor(ECC_Visibility,false, HitResult);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.f, 12, FColor::Blue, false, -1.f);
		RotateToTarget(HitResult.ImpactPoint);
	}
	
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction,					ETriggerEvent::Triggered, this, &ASCharacter::Move					);
		//EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this, &ASCharacter::Rotate);

		// GAS
		EnhancedInputComponent->BindAction(PrimaryAttackAction,			ETriggerEvent::Triggered, this, &ASCharacter::OnPrimaryAttack			);
		EnhancedInputComponent->BindAction(SecondaryAttackAction,		ETriggerEvent::Triggered, this, &ASCharacter::OnSecondaryAttack		);
		EnhancedInputComponent->BindAction(MovementAbilityAction,		ETriggerEvent::Triggered, this, &ASCharacter::OnMovementAbility		);
		EnhancedInputComponent->BindAction(InteractionAbilityAction,	ETriggerEvent::Triggered, this, &ASCharacter::OnInteractionAbility	);
		EnhancedInputComponent->BindAction(UtilityAbilityAction,		ETriggerEvent::Triggered, this, &ASCharacter::OnUtilityAbility		);
	}
	
}

UAbilitySystemComponent* ASCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

void ASCharacter::InitAbilitySystemComponent()
{
	ASPlayerState* PState = GetPlayerState<ASPlayerState>();
	if (!PState)
	{
		return;
	}

	AbilitySystemComponent = PState->GetAbilitySystemComponent();
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}

	AbilitySystemComponent->InitAbilityActorInfo(PState, this);
}

void ASCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilitySystemComponent();
}

void ASCharacter::OnPrimaryAttack(const FInputActionValue& Value)
{
	SendAbilityLocalInput(Value, static_cast<int32>(EAbilityInputID::PrimaryAttack));
}

void ASCharacter::OnSecondaryAttack(const FInputActionValue& Value)
{
	SendAbilityLocalInput(Value, static_cast<int32>(EAbilityInputID::SecondaryAttack));
}

void ASCharacter::OnMovementAbility(const FInputActionValue& Value)
{
	SendAbilityLocalInput(Value, static_cast<int32>(EAbilityInputID::MovementAbility));
}

void ASCharacter::OnInteractionAbility(const FInputActionValue& Value)
{
	SendAbilityLocalInput(Value, static_cast<int32>(EAbilityInputID::InteractionAbility));
}

void ASCharacter::OnUtilityAbility(const FInputActionValue& Value)
{
	SendAbilityLocalInput(Value, static_cast<int32>(EAbilityInputID::UtilityAbility));
}

void ASCharacter::SendAbilityLocalInput(const FInputActionValue& Value, int32 InputID)
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}

	if (Value.Get<bool>())
	{
		AbilitySystemComponent->AbilityLocalInputPressed(InputID);
	}
	else
	{
		AbilitySystemComponent->AbilityLocalInputReleased(InputID);
	}
}

