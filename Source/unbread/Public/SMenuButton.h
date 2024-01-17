// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SMenuButton.generated.h"

/**
 * 
 */
class UButton;

UCLASS()
class UNBREAD_API USMenuButton : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UButton* Button;
	
};
