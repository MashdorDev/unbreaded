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
#include "SExplodingBody.h"
#include "SPlayerState.h"
#include "SGameplayAbility.h"
#include "SHealthAttributeSet.h"
#include "Camera/CameraActor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ASCharacter::ASCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(34.f, 88.f);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetCapsuleComponent());

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	DynamicCamera = CreateDefaultSubobject<UDynamicCameraComponent>("DynamicCamera");

	// TODO: ADD PROJECTILE SPAWN POINT

	// TEMPORARY
	bIsJumping = false;
	JumpCount = 0;

	WalkSpeed = 0.5f;
	SprintSpeed = 1.0f;
	Speed = WalkSpeed;
	bIsWalking = true;
	
	MaxAmmo = 300;
	CurrentAmmo = 300;

}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Add Enhanced Input Mapping Context
	PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultInputMappingContext, 0);
		}
	}

	APlayerCameraManager* const camMan = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	camMan->ViewPitchMax = -50.0f;
	camMan->ViewPitchMax = 10.0f;

	ASPlayerState* PState = GetPlayerState<ASPlayerState>();
	if (!PState)
	{
		return;
	}

	// Hook Up Delegates
	USHealthAttributeSet* HealthAttributeSet = PState->HealthAttributeSet;

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthAttributeSet->GetHealthAttribute()).AddUObject(this, &ASCharacter::OnHealthAttributeChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthAttributeSet->GetShieldAttribute()).AddUObject(this, &ASCharacter::OnShieldAttributeChanged);
}

void ASCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MoveVector = Value.Get<FVector2D>();
	FVector2d NormalizedMoveVector = MoveVector.GetSafeNormal();

	FVector Forward, Right;
	
	// Get the camera transform
	if(DynamicCamera->CurrentCameraActor)
	{
		const FRotator CameraWorldRotation = DynamicCamera->CurrentCameraActor->GetComponentByClass<UCameraComponent>()->GetComponentRotation();
		Right = UKismetMathLibrary::GetRightVector(CameraWorldRotation); //Forward.RotateAngleAxis(90.f, FVector(0.f, 0.f, 1.f));
		Forward = Right.RotateAngleAxis(-90.f, FVector(0.f, 0.f, 1.f));
	}

	else
	{
		Forward = FVector(1.f, 0.f, 0.f);
		Right = FVector(0.f, 1.f, 0.f);
	}
	
	// Forward / Backward
	AddMovementInput(Forward, NormalizedMoveVector.Y * Speed);
	
	// Right / Left
	AddMovementInput(Right, NormalizedMoveVector.X * Speed);

	// TODO: Update forward and right vectors according to camera position and rotation
	//
	//
}

void ASCharacter::Rotate(const FInputActionValue& Value)
{
	const FVector2D RotVector = Value.Get<FVector2D>();
	const float Angle = FMath::Atan2(RotVector.Y, RotVector.X) * (180.0f / PI);

	// Rotate the character relative to the current camera
	FRotator CameraWorldRotation = DynamicCamera->CurrentCameraActor->GetComponentByClass<UCameraComponent>()->GetComponentRotation();
	CameraWorldRotation.Roll = 0.f;
	CameraWorldRotation.Pitch = 0.f;	
	const FRotator TargetRotation = UKismetMathLibrary::ComposeRotators(FRotator(0.0f, -1* Angle, 0.0f), CameraWorldRotation);

	const float LerpSpeed = 0.1f;

	FRotator LerpedRotation = FMath::Lerp(GetMesh()->GetRelativeTransform().GetRotation().Rotator(), TargetRotation, LerpSpeed);
	
	GetMesh()->SetWorldRotation(LerpedRotation);
}

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

	/*const FVector ToTarget = LookAtTarget - GetMesh()->GetComponentLocation(); // this is a world rotation
	const FRotator LookAtRotation(0.f, ToTarget.Rotation().Yaw - 90.f, 0.f); //

	GetMesh()->SetWorldRotation(FMath::RInterpTo(GetMesh()->GetComponentRotation(), LookAtRotation, UGameplayStatics::GetWorldDeltaSeconds(this), 10.f));*/

	// METHOD 4


	
	// TODO: Update rotation according to camera, lerp as tank
}

void ASCharacter::CheckJump()
{
	if (bIsJumping)
	{
		bIsJumping = false;
	}
	else
	{
		bIsJumping = true;
		JumpCount++;
		if (JumpCount == 2)
		{
			LaunchCharacter(FVector(0.f, 0.f, 600.f), false, true);
		}
	}
}


void ASCharacter::Jump(const FInputActionValue& Value)
{
	ACharacter::Jump();
}

void ASCharacter::Sprint()
{
	if(bIsHeadForm)
	{
		return;
	}
	
	bIsWalking = !bIsWalking;
	if (bIsWalking)
	{
		Speed = WalkSpeed;
	}
	else
	{
		Speed = SprintSpeed;
	}
}

void ASCharacter::CheckAmmo()
{
	if (CurrentAmmo > 0)
	{
		ShootProjectile();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Ammo!"));
	}
}

void ASCharacter::ShootProjectile()
{
	if(bIsHeadForm)
	{
		return;
	}
	
	FVector ProjectileSpawnLocation = GetMesh()->GetSocketLocation("ProjectileSpawn") + FVector(0.f, 0.f, 150.f);
	FRotator ProjectileSpawnRotation = GetMesh()->GetRelativeRotation() + FRotator(0.0f, 90.f, -10.f);
	FTransform SpawnTM = FTransform(ProjectileSpawnRotation, ProjectileSpawnLocation);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	CurrentAmmo--;
	UE_LOG(LogTemp, Log, TEXT("Ammo Remaining: %d"), CurrentAmmo);
}

void ASCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	JumpCount = 0;
}

void ASCharacter::SetNextCamera_Implementation(AActor* CameraActor)
{
	IDynamicCameraInterface::SetNextCamera_Implementation(CameraActor);
	DynamicCamera->SetNextCamera(CameraActor);
}

void ASCharacter::TransitionCamera_Implementation(const float TransitionTime)
{
	IDynamicCameraInterface::TransitionCamera_Implementation(TransitionTime);
	DynamicCamera->TransitionCamera(TransitionTime);
}

void ASCharacter::LaunchHead()
{
	bIsHeadForm = true;
	
	// Store the current location and rotation of the character
	const FVector BodySpawnLocation = GetMesh()->GetComponentLocation();
	const FRotator BodySpawnRotation = GetMesh()->GetComponentRotation();

	// Swap the mesh and launch the head
	GetMesh()->SetSkeletalMeshAsset(HeadMesh);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -150.f));
	GetCapsuleComponent()->SetCapsuleHalfHeight(34.f);

	const FVector LaunchVelocity = GetMesh()->GetRightVector() * HeadLaunchVelocityMultiplier;
	
	GetCharacterMovement()->Velocity = LaunchVelocity;

	// Spawn the body and add it to ActiveBodies
	ActiveBodies.AddUnique(GetWorld()->SpawnActor<ASExplodingBody>(BodyClass, BodySpawnLocation, BodySpawnRotation));
	
}

void ASCharacter::DestroyBodyAndSpawnCrumbles()
{
	for (auto& body : ActiveBodies)
	{
		body->Explode();
	}
	ActiveBodies.Empty();
}

void ASCharacter::ReformBody()
{
	// If there is not a crumble pile nearby return
	if(!bIsHeadForm)
	{
		return;
	}

	bIsHeadForm = false;

	DestroyBodyAndSpawnCrumbles();

	AddActorWorldOffset(FVector(0.f, 0.f, 90.f));
	
	GetMesh()->SetSkeletalMeshAsset(CharacterMesh);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));

	GetCapsuleComponent()->SetCapsuleHalfHeight(88.f);

	// Crumble Pile Call CrumbleInteraction()
}


// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Using APlayerController::GetHitResultUnderCursor to line trace to mouse cursor and getting hit information
	/*if (PlayerController)
	{
		// We're passing the FHitResult as reference but not const, because we need to change the information on HitResult with every hit.
		/*FHitResult HitResult;
		PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.f, 12, FColor::Blue, false, -1.f);
		RotateToTarget(HitResult.ImpactPoint);#1#
	}*/

	if (bIsJumping)
	{
		ACharacter::Jump();
	}

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASCharacter::Move);
		EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this, &ASCharacter::Rotate);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ASCharacter::StopJumping);

		// TEMPORARY
		EnhancedInputComponent->BindAction(ProjectileAttackAction, ETriggerEvent::Triggered, this, &ASCharacter::CheckAmmo);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ASCharacter::Sprint);

		// GAS
		EnhancedInputComponent->BindAction(PrimaryAttackAction, ETriggerEvent::Triggered, this, &ASCharacter::OnPrimaryAttack);
		EnhancedInputComponent->BindAction(SecondaryAttackAction, ETriggerEvent::Triggered, this, &ASCharacter::OnSecondaryAttack);
		EnhancedInputComponent->BindAction(MovementAbilityAction, ETriggerEvent::Triggered, this, &ASCharacter::OnMovementAbility);
		EnhancedInputComponent->BindAction(InteractionAbilityAction, ETriggerEvent::Triggered, this, &ASCharacter::OnInteractionAbility);
		EnhancedInputComponent->BindAction(UtilityAbilityAction, ETriggerEvent::Triggered, this, &ASCharacter::OnUtilityAbility);
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

	InitializeAbilities();
	InitializeEffects();
}

void ASCharacter::InitializeAbilities()
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}

	// Iterate and give abilities to all
	for (TSubclassOf<USGameplayAbility>& Ability : DefaultAbilities)
	{
		FGameplayAbilitySpecHandle SpecHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, 1, static_cast<int32>(Ability.GetDefaultObject()->AbilityInputID), this));
		GivenAbilites.Add(SpecHandle);
	}
}

void ASCharacter::InitializeEffects()
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect>& Effect : DefaultEffects)
	{
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, 1, EffectContext);
		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle GEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void ASCharacter::ClearGivenAbilities()
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}

	for (FGameplayAbilitySpecHandle AbilitySpecHandle : GivenAbilites)
	{
		AbilitySystemComponent->ClearAbility(AbilitySpecHandle);
	}
}

void ASCharacter::OnHealthAttributeChanged(const FOnAttributeChangeData& Data)
{
	OnHealthChanged(Data.OldValue, Data.NewValue);
}

void ASCharacter::OnShieldAttributeChanged(const FOnAttributeChangeData& Data)
{
	OnShieldChanged(Data.OldValue, Data.NewValue);
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