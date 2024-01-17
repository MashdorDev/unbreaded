// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGraphPalette.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "SMenuButton.h"
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
	TArray<USMenuButton*> Buttons;
	

	
};
