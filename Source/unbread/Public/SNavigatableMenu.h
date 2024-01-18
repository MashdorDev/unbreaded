// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGraphPalette.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "SMenuButton.h"
#include "ButtonWidgetStyle.generated.h"
#include "SNavigatableMenu.generated.h"

/**
 * 
 */
class UButton;


UCLASS()
class UNBREAD_API USNavigatableMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TMap<FString, USMenuButton*> Buttons;

	UPROPERTY(EditAnywhere)
	USMenuButton* Selected;

	

	UFUNCTION(BlueprintCallable)
	void AddButton(USMenuButton* Button);

	UFUNCTION(BlueprintCallable)
	void Navigate(EDirection Direction);

	UFUNCTION(BlueprintCallable)
	void AddConnection(USMenuButton* FromButton, USMenuButton* ToButton, EDirection Direction);

	

	UFUNCTION(BlueprintCallable)
	void SetSelected(USMenuButton* Selected_);
	
};
