// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Add Enhanced Input Mapping Context
	if(APlayerController* PlayerController = Cast<APlayerController>(GetController()))
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
	AddMovementInput(Forward, MoveVector.Y * MoveSpeed);
	
	const FVector Right = FVector(0.f, 1.f,0.f);
	AddMovementInput(Right, MoveVector.X * RotationRate);

	// TODO: Update forward and right vectors according to camera position and rotation
	//
	//
}

void ASCharacter::Rotate(const FInputActionValue& Value)
{
	//const FRotator Rotation = Controller->GetControlRotation();
	//const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	//FRotator DeltaRotation = FRotator::ZeroRotator;
	// FRotator DeltaRotation = Value.Get<FRotator>();
	// DeltaRotation.Yaw *= RotationRate * UGameplayStatics::GetWorldDeltaSeconds(this);
	// AddActorLocalRotation(DeltaRotation, true);

	const FVector2D RotateAxisValue = Value.Get<FVector2D>();
	if(GetController())
	{
		AddControllerYawInput(RotateAxisValue.X);
		AddControllerPitchInput(RotateAxisValue.Y);
	}	

	// TODO: Update rotation according to camera, lerp as tank
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASCharacter::Move);
		EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this, &ASCharacter::Rotate);
	}


}

