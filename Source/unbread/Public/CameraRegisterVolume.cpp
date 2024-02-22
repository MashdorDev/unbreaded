// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraRegisterVolume.h"
#include "DynamicCameraInterface.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"


// Sets default values
ACameraRegisterVolume::ACameraRegisterVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Create and set the Root component
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	// Create the camera component and attach it to the Root
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(Root);

	// Create the trigger volume and attach it to the Root
	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Volume"));
	TriggerVolume->SetupAttachment(Root);
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &ACameraRegisterVolume::OnEnterVolume);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &ACameraRegisterVolume::OnExitVolume);

}

// Called when the game starts or when spawned
void ACameraRegisterVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACameraRegisterVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACameraRegisterVolume::OnEnterVolume(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->GetClass()->ImplementsInterface(UDynamicCameraInterface::StaticClass()))	{
		IDynamicCameraInterface::Execute_SetNextCamera(OtherActor, this);
		IDynamicCameraInterface::Execute_TransitionCamera(OtherActor, CameraTransitionTime);
	}
}

void ACameraRegisterVolume::OnExitVolume(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor->GetClass()->ImplementsInterface(UDynamicCameraInterface::StaticClass()))	{
		if(bResetCameraOnExit)
		{
			IDynamicCameraInterface::Execute_SetNextCamera(OtherActor, nullptr);
		}
		IDynamicCameraInterface::Execute_TransitionCamera(OtherActor, CameraTransitionTime);
	}
}
