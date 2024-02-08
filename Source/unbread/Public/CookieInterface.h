// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CookieInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCookieInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNBREAD_API ICookieInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Camera Interface Events")
	void SetCanRevert(AActor* CameraActor);
};
