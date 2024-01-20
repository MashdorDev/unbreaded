// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DynamicCameraInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDynamicCameraInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNBREAD_API IDynamicCameraInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Camera Interface Events")
	void SetNextCamera(AActor* CameraActor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Camera Interface Events")
	void TransitionCamera(const float TransitionTime);
};
