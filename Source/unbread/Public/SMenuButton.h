// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "SMenuButton.generated.h"

/**
 * 
 */
class UButton;

UENUM()
enum EDirection { Up = 0, Right, Down, Left };


UCLASS()
class UNBREAD_API USMenuButton : public UCanvasPanel
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UButton* Button;
	UPROPERTY(EditAnywhere)
	TArray<FString> Connections;
	
	
	UPROPERTY(EditAnywhere)
	FString Name;
	
	UFUNCTION()
	void AddConnection(EDirection Direction_, FString Name_);

	
};
