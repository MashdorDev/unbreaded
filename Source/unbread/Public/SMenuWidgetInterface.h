// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SMenuWidgetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class USMenuWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNBREAD_API ISMenuWidgetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetChildWidget(const TScriptInterface<ISMenuWidgetInterface>& Child);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OpenChildWidget();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void CloseChildWidget();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetParentWidget(const TScriptInterface<ISMenuWidgetInterface>& Parent);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OpenParentWidget();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void CloseParentWidget();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ToggleVisibility(bool Visible);
	
};
