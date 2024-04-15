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
#include "SCrumbles.h"
#include "SExplodingBody.h"
#include "SPlayerState.h"
#include "SGameplayAbility.h"
#include "SHealthAttributeSet.h"
#include "SInteractionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "SWeapon.h"

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

	InteractionComponent = CreateDefaultSubobject<USInteractionComponent>("InteractionComponent");

	// TEMPORARY
	bIsJumping = false;
	GravityAppliedOnWalk = 3.f;
	GravityAppliedOnFall = 6.f;
	JumpBufferDuration = 0.1f;
	LaunchHeadMaxDuration = 1.f;
	
	BodySpeed = 0.8;
	HeadSpeed = 1.0f;
	Speed = BodySpeed;

	RollAngleMin = -30.f;
	RollAngleMax = 30.f;

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

	camMan = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	camMan->ViewPitchMax = -50.0f;
	camMan->ViewPitchMax = 10.0f;
	
	ASPlayerState* PState = GetPlayerState<ASPlayerState>();
	if (!PState)
	{
		return;
	}

	// Set Gravity
	//GetCharacterMovement()->GravityScale = GravityAppliedOnWalk;

	// Hook Up Delegates
	USHealthAttributeSet* HealthAttributeSet = PState->HealthAttributeSet;

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthAttributeSet->GetHealthAttribute()).AddUObject(this, &ASCharacter::OnHealthAttributeChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(HealthAttributeSet->GetShieldAttribute()).AddUObject(this, &ASCharacter::OnShieldAttributeChanged);
	
	// Spawn and Equip the default weapon for Player Character
	UWorld* const World = GetWorld();

	if (World && DefaultWeaponClass)
	{
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ActorSpawnParams.Owner = this;

		ASWeapon* DefaultWeapon = World->SpawnActor<ASWeapon>(DefaultWeaponClass, GetActorLocation(), GetActorRotation(), ActorSpawnParams);
		EquipWeapon(DefaultWeapon);
	}
}

void ASCharacter::MoveStopped(const FInputActionValue& Value)
{
	//bCanRotateCamera = true;
}

void ASCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MoveVector = Value.Get<FVector2D>();
	FVector2d NormalizedMoveVector = MoveVector.GetSafeNormal();
	float MoveVectorLength = MoveVector.Size();

	float AdjSpeed = Speed * MoveVectorLength;
	
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
	AddMovementInput(Forward, NormalizedMoveVector.Y * AdjSpeed);
	
	// Right / Left
	AddMovementInput(Right, NormalizedMoveVector.X * AdjSpeed);
	
	if(!bUseNewRotation) return;

	const FVector2D RotVector = Value.Get<FVector2D>();
	
	const float Angle = FMath::Atan2(RotVector.Y, RotVector.X) * (180.0f / PI);

	// Rotate the character relative to the current camera
	FRotator CameraWorldRotation = DynamicCamera->CurrentCameraActor->GetComponentByClass<UCameraComponent>()->GetComponentRotation();
	CameraWorldRotation.Roll = 0.f;
	CameraWorldRotation.Pitch = 0.f;	
	const FRotator TargetRotation = UKismetMathLibrary::ComposeRotators(FRotator(0.0f, -1* Angle, 0.0f), CameraWorldRotation);
	
	FRotator LerpedRotation = FMath::Lerp(GetMesh()->GetComponentRotation(), TargetRotation, LerpSpeed);
	
	GetMesh()->SetWorldRotation(LerpedRotation);
	//bCanRotateCamera = false;
}

void ASCharacter::StopedRotate(const FInputActionValue& Value)
{
	bCanRotateCamera = true;
}

void ASCharacter::Rotate(const FInputActionValue& Value)
{
	const FVector2D RotVector = Value.Get<FVector2D>();	
	const float YawAngle = RotVector.X;	
	const float RollAngle = RotVector.Y;
	
	FRotator CameraRotation{RollAngle, YawAngle * CameraRotationMultiplier, 0.f};
	SpringArmComponent->AddRelativeRotation(CameraRotation);

	FRotator ClampedRotation = SpringArmComponent->GetRelativeRotation();
	ClampedRotation.Pitch = FMath::Clamp(SpringArmComponent->GetRelativeRotation().Pitch, RollAngleMin, RollAngleMax);
	SpringArmComponent->SetRelativeRotation(ClampedRotation);
	bCanRotateCamera = false;
	/*
	if(bUseNewRotation) return;
	
	const FVector2D RotVector = Value.Get<FVector2D>();
	
	const float Angle = FMath::Atan2(RotVector.Y, RotVector.X) * (180.0f / PI);

	// Rotate the character relative to the current camera
	FRotator CameraWorldRotation = DynamicCamera->CurrentCameraActor->GetComponentByClass<UCameraComponent>()->GetComponentRotation();
	CameraWorldRotation.Roll = 0.f;
	CameraWorldRotation.Pitch = 0.f;	
	const FRotator TargetRotation = UKismetMathLibrary::ComposeRotators(FRotator(0.0f, -1* Angle, 0.0f), CameraWorldRotation);
	
	FRotator LerpedRotation = FMath::Lerp(GetMesh()->GetComponentRotation(), TargetRotation, LerpSpeed);
	
	GetMesh()->AddWorldRotation(LerpedRotation);
	*/
}

void ASCharacter::Jump(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("JUMP!"));
 	if (!bIsJumping)
	{
		Super::Jump();	
	}
	else if (bIsJumping)
	{
		BufferJump();
	}
}

void ASCharacter::StopJumping()
{

	Super::StopJumping();
	if (!GetCharacterMovement()->IsWalking() && !bIsOverlappingPad)
	{
		GetCharacterMovement()->GravityScale = GravityAppliedOnFall;
	}
	
}

void ASCharacter::BufferJump()
{
	UE_LOG(LogTemp, Warning, TEXT("JUMP BUFFERED!"));
	bJumpBuffered = true;
	GetWorldTimerManager().SetTimer(JumpBufferTimer, this, &ASCharacter::UnBufferJump, JumpBufferDuration);
}

void ASCharacter::UnBufferJump()
{
	UE_LOG(LogTemp, Error, TEXT("JUMP UN-BUFFERED!"));
	bJumpBuffered = false;
	GetWorldTimerManager().ClearTimer(JumpBufferTimer);
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

void ASCharacter::SetNearestCrumblePile_Implementation(AActor* CrumblesActor)
{
	ICookieInterface::SetNearestCrumblePile_Implementation(CrumblesActor);
	NearestCrumbles = CrumblesActor;
}

void ASCharacter::LaunchHead()
{

	if(bIsLevelSequencePlaying)
	{
		return;
	}
	
	bIsHeadForm = true;
	
	FVector Start = GetMesh()->GetComponentLocation() + FVector(0.f, 0.f, 50.f);
	FVector End = GetMesh()->GetComponentLocation() + FVector(0.f, 0.f, 50.f) + -GetMesh()->GetRightVector() * 100;
	FHitResult OutHit;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(this);
	
	const bool isCollision = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Start, End, 20.0f,
		ETraceTypeQuery::TraceTypeQuery_MAX, false, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true, FColor::Green);

	FVector BodySpawnLocation ;

	// Store the current location and rotation of the character
	if(isCollision)
	{
		BodySpawnLocation = GetMesh()->GetComponentLocation() +
			 FVector(0.f, 0.f, 50.f) + GetMesh()->GetRightVector() * 140;
	}
	else
	{
 		BodySpawnLocation = GetMesh()->GetComponentLocation() + FVector(0.f, 0.f, 50.f) + -GetMesh()->GetRightVector() * 20;
	}
	FRotator BodySpawnRotation =  GetMesh()->GetComponentRotation();

	
	// Swap the mesh and launch the head
	GetMesh()->SetSkeletalMeshAsset(HeadMesh);

	// Move the head down & Adjust the collider
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -150.f)); 
	GetCapsuleComponent()->SetCapsuleHalfHeight(34.f);

	// Move head forward & launch
	AddActorWorldOffset(GetMesh()->GetRightVector() * 50.f, true);
	
	FVector LaunchVelocity = ((GetMesh()->GetRightVector()) * HeadLaunchVelocityMultiplier);
	LaunchVelocity.Z += HeadLaunchVelocityZAxisAdd;
	//GetCharacterMovement()->Velocity = LaunchVelocity;

	GetCharacterMovement()->FallingLateralFriction = 0;
	GetCharacterMovement()->GravityScale = GravityAppliedOnWalk;
	GetCharacterMovement()->Launch(LaunchVelocity);
	GetWorldTimerManager().SetTimer(LaunchHeadTimerHandle, this, &ASCharacter::ResetLaunchHeadTimer, LaunchHeadMaxDuration);

	
	// Spawn the body and add it to ActiveBodies
	FActorSpawnParameters Parameters {};
	Parameters.bNoFail = true;
	auto Spawned = GetWorld()->SpawnActor<ASExplodingBody>(BodyClass, BodySpawnLocation, BodySpawnRotation, Parameters);
	FVector Impulse = isCollision ? GetMesh()->GetRightVector() : -GetMesh()->GetRightVector();
	Spawned->Mesh->AddImpulse(Impulse * 10 * HeadLaunchVelocityMultiplier);
	Spawned->SetInstigator(this);
	ActiveBodies.AddUnique(Spawned);
	
	Speed = HeadSpeed;
	
}

void ASCharacter::LaunchHeadVertical()
{
	if(bIsLevelSequencePlaying)
	{
		return;
	}
	
	bIsHeadForm = true;
	
	FVector Start = GetMesh()->GetComponentLocation() + FVector(0.f, 0.f, 50.f);
	FVector End = GetMesh()->GetComponentLocation() + FVector(0.f, 0.f, 50.f) + -GetMesh()->GetRightVector() * 100;
	FHitResult OutHit;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(this);
	
	const bool isCollision = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Start, End, 20.0f,
		ETraceTypeQuery::TraceTypeQuery_MAX, false, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true, FColor::Green);

	FVector BodySpawnLocation ;

	// Store the current location and rotation of the character
	if(isCollision)
	{
		BodySpawnLocation = GetMesh()->GetComponentLocation() +
			 FVector(0.f, 0.f, 50.f) + GetMesh()->GetRightVector() * 140;
	}
	else
	{
 		BodySpawnLocation = GetMesh()->GetComponentLocation() + FVector(0.f, 0.f, 50.f) + -GetMesh()->GetRightVector() * 20;
	}
	FRotator BodySpawnRotation =  GetMesh()->GetComponentRotation();

	
	// Swap the mesh and launch the head
	GetMesh()->SetSkeletalMeshAsset(HeadMesh);

	// Move the head down & Adjust the collider
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -150.f)); 
	GetCapsuleComponent()->SetCapsuleHalfHeight(34.f);

	// Move head forward & launch
	AddActorWorldOffset(GetMesh()->GetUpVector() * 120.f, true);
	
	FVector LaunchVelocity = ((GetMesh()->GetRightVector()) * HeadLaunchVelocityMultiplier);
	LaunchVelocity.X = 0;
	LaunchVelocity.Y = 0;
	LaunchVelocity.Z += HeadLaunchVelocityZAxisAdd * 0.8;
	//GetCharacterMovement()->Velocity = LaunchVelocity;

	GetCharacterMovement()->FallingLateralFriction = 0;
	GetCharacterMovement()->GravityScale = GravityAppliedOnWalk;
	GetCharacterMovement()->Launch(LaunchVelocity);
	GetWorldTimerManager().SetTimer(LaunchHeadTimerHandle, this, &ASCharacter::ResetLaunchHeadTimer, LaunchHeadMaxDuration);

	
	// Spawn the body and add it to ActiveBodies
	FActorSpawnParameters Parameters {};
	Parameters.bNoFail = true;
	auto Spawned = GetWorld()->SpawnActor<ASExplodingBody>(BodyClass, BodySpawnLocation, BodySpawnRotation, Parameters);
	FVector Impulse = isCollision ? GetMesh()->GetRightVector() : -GetMesh()->GetRightVector();
	Spawned->Mesh->AddImpulse(Impulse * 10 * HeadLaunchVelocityMultiplier);
	Spawned->SetInstigator(this);
	ActiveBodies.AddUnique(Spawned);
	
	Speed = HeadSpeed;
	
}

void ASCharacter::DestroyBodyAndSpawnCrumbles()
{
	for (auto& Body : ActiveBodies)
	{
		if(Body)
		{
			Body->Explode();
		}
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

	if(!NearestCrumbles)
	{
		return;
	}

	//NearestCrumbles->Destroy();

	if(NearestCrumbles->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
	{
		IInteractInterface::Execute_CrumbleInteraction(NearestCrumbles, this);
	}
	NearestCrumbles = nullptr;
	bIsHeadForm = false;
	Speed = BodySpeed;
	
	DestroyBodyAndSpawnCrumbles();

	AddActorWorldOffset(FVector(0.f, 0.f, 90.f));
	
	GetMesh()->SetSkeletalMeshAsset(CharacterMesh);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -90.f));

	GetCapsuleComponent()->SetCapsuleHalfHeight(88.f);
}


void ASCharacter::ResetLaunchHeadTimer()
{
	GetCharacterMovement()->FallingLateralFriction = 4.0f;
}

/*void ASCharacter::MeleeInteract()
{
	if (ensure(InteractionComponent))
	{
		InteractionComponent->MeleeInteract();	
	}
}*/

/*void ASCharacter::Punch()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	
	FVector EyeLocation;
	FRotator EyeRotation;
	GetMesh()->GetSocketWorldLocationAndRotation(TEXT("spine_004"), EyeLocation, EyeRotation);
	FRotator Correction (0.0f, 90.0f, 0.f);
	FRotator CorrecterRotation = EyeRotation + Correction;
	FVector End = EyeLocation + (CorrecterRotation.Vector() * 500);

	TArray<FHitResult> Hits;

	float DebugRadius = 30.f;

	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(DebugRadius);

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, CollisionShape);
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
	
	/*for (FHitResult Hit : Hits)
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			if(HitActor->Implements<USGameplayAbility>())
			{
				APawn* MyPawn = Cast<APawn>(this);
				ISGameplayInterface::Execute_Interact(HitActor, MyPawn);
				break;
			}
		}
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, DebugRadius, 16, LineColor, false, 2.0f);
	}#1#
	DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);
	
}*/

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	/*
	 * Deprecated Occlusion Mask; using Post+Processing instead
	 *

	TArray<FHitResult> OutHit;

	TArray<FHitResult> OutHits;
	FVector CamLocation = camMan->GetCameraLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(this);

	FVector Start = GetActorLocation();
	Start.Z += GetCapsuleComponent()->GetScaledCapsuleHalfHeight() / 2;

	FVector End = CamLocation + (CamLocation - GetActorLocation()).GetSafeNormal() * GetCapsuleComponent()->GetScaledCapsuleRadius();

	TArray<TEnumAsByte<EObjectTypeQuery>> CollisionObjectTypes;

#define CAMERA_OCCLUSION_CHANNEL ECollisionChannel::ECC_EngineTraceChannel4
	CollisionObjectTypes.Add(UEngineTypes::ConvertToObjectType(CAMERA_OCCLUSION_CHANNEL));

	//CollisionObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	//CollisionObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
	
	bool isCollision = UKismetSystemLibrary::CapsuleTraceMultiForObjects(
	GetWorld(), Start, End, 1,
	GetCapsuleComponent()->GetScaledCapsuleHalfHeight() / 2, CollisionObjectTypes, true,
	ActorsToIgnore,
	EDrawDebugTrace::None,
	OutHits, true);
	
	TArray<const UStaticMeshComponent*> MeshesJustOccluded;
	if(isCollision && OutHits.Num() > 0)
	{
		for(int i = 0; i < OutHits.Num(); ++i)
		{
			UStaticMeshComponent* HitComponent = Cast<UStaticMeshComponent>(OutHits[i].GetComponent());
			if(!HitComponent || HitComponent->GetOwner()->GetInstigator())
			{
				return;
			}
			
			HideOccludedActor(HitComponent);
			MeshesJustOccluded.AddUnique(HitComponent);
		}
	}
	for(auto& [Key, Value]: OccludedActors)
	{
		if(!MeshesJustOccluded.Contains(Value.StaticMesh) && Value.IsOccluded)
		{
			if (!IsValid(Value.Actor))
			{
				OccludedActors.Remove(Value.StaticMesh);
			}
			Value.IsOccluded = false;
			for(int i = 0; i < Value.Materials.Num(); i++)
			{
				Value.StaticMesh->SetMaterial(i, Value.Materials[i]);
			}
		}
	}
	*/
}

void ASCharacter::HideOccludedActor(UStaticMeshComponent* OccludedMesh)
{
	if(OccludedMesh)
	{
		FCameraOccludedActor* ExistingActor = OccludedActors.Find(OccludedMesh);
		if(ExistingActor && ExistingActor->IsOccluded)
		{
			return;
		}
		if(ExistingActor && ExistingActor->StaticMesh == OccludedMesh)
		{
			ExistingActor->IsOccluded = true;
			for(int i = 0; i < ExistingActor->StaticMesh->GetNumMaterials(); i++)
			{
				ExistingActor->StaticMesh->SetMaterial(i, FadeMaterial);
			}
		}
		else
		{
			FCameraOccludedActor OccludedActor;
			OccludedActor.Actor = OccludedMesh->GetOwner();
			OccludedActor.Materials = OccludedMesh->GetMaterials();
			OccludedActor.StaticMesh = OccludedMesh;
			OccludedActor.IsOccluded = true;
			OccludedActors.Add(OccludedMesh, OccludedActor);

			for(int i = 0; i < OccludedActor.StaticMesh->GetNumMaterials(); i++)
			{
				OccludedActor.StaticMesh->SetMaterial(i, FadeMaterial);
			}
		}
	}
}

void ASCharacter::ForceShowActors()
{
	for(auto& [Key, Value]: OccludedActors)
	{
		if (!IsValid(Value.Actor))
		{
			OccludedActors.Remove(Value.StaticMesh);
		}
		Value.IsOccluded = false;
		for(int i = 0; i < Value.Materials.Num(); i++)
		{
			Value.StaticMesh->SetMaterial(i, Value.Materials[i]);
		}
	}
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ASCharacter::MoveStopped);
		EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this, &ASCharacter::Rotate);
		EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Completed, this, &ASCharacter::StopedRotate);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ASCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ASCharacter::StopJumping);
		
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
	if (!HasAuthority() || !AbilitySystemComponent.IsValid())
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

void ASCharacter::EquipWeapon(ASWeapon* NewWeapon)
{
	if (!NewWeapon)
	{
		return;
	}

	if (Weapon)
	{
		Weapon->SetActorLocation(NewWeapon->GetActorLocation());
		Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Weapon->UnEquip();
	}

	Weapon = NewWeapon;
	Weapon->SetOwner(this);
	Weapon->SetActorLocation(GetActorLocation());
	FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
	Weapon->AttachToComponent(GetMesh(), AttachmentTransformRules);
	Weapon->Equip();
	Weapon->SetActorHiddenInGame(true);
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