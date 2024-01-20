// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(34.f, 88.f);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	BaseMesh->SetupAttachment(GetMesh());
	
	ForwardDirectionIndicatorMesh = CreateDefaultSubobject<UStaticMeshComponent>("ForwardDirectionIndicatorMesh");
	ForwardDirectionIndicatorMesh->SetupAttachment(BaseMesh);

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
	
	const FVector Forward = FVector(1.f, 0.f,0.f);
	AddMovementInput(Forward, MoveVector.Y);
	
	const FVector Right = FVector(0.f, 1.f,0.f);
	AddMovementInput(Right, MoveVector.X);

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

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector2D v(0.f);
	FVector2d s(0.f);
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
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASCharacter::Move);
		//EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this, &ASCharacter::Rotate);
	}


}

