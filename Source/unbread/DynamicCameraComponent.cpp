// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicCameraComponent.h"

#include "CameraRegisterVolume.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UDynamicCameraComponent::UDynamicCameraComponent() : DefaultCameraActor(nullptr), NextCameraActor(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDynamicCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...



	
	// Set The Default Camera
	FindDefaultLevelCamera();	
}


// Called every frame
void UDynamicCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Sets the camera that will be transitioned to next
void UDynamicCameraComponent::SetNextCamera(AActor* CameraActor)
{
	
	if(CameraActor == nullptr)
	{
		NextCameraActor = DefaultCameraActor;
	}
	else
	{
		NextCameraActor = CameraActor; 
	}
}

// Transitions to the next camera
void UDynamicCameraComponent::TransitionCamera(const float TransitionTime)
{
	// Do nothing if there is no camera actor to transition to
	if(NextCameraActor == nullptr)
	{
		if(DefaultCameraActor == nullptr)
		{
			return;
		}
		
		NextCameraActor = DefaultCameraActor;
	}

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	// Transition to the NextCameraActor 
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTargetWithBlend(NextCameraActor, TransitionTime);
	CurrentCameraActor = NextCameraActor;
	//NextCameraActor = nullptr;

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	return;
}

void UDynamicCameraComponent::FindDefaultLevelCamera()
{
	TArray<AActor*> CameraActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "MainCamera", CameraActors);
	//AActor::GetOverlappingActors()

	// Get the first actor tagged as "MainCamera"
	if(CameraActors.Num() > 0)
	{
		DefaultCameraActor = CameraActors[0];

		if(auto MainCamera = Cast<ACameraRegisterVolume>(CameraActors[0]))
		{
			MainCamera->Target = GetOwner();
		}
	}
	else
	{
		DefaultCameraActor = GetOwner();
	}

	TransitionCamera(0.f);
}

