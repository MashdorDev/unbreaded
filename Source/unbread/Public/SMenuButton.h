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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMenuButtonHovered, USMenuButton*, Button);

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

	UPROPERTY(BlueprintAssignable)
	FOnMenuButtonHovered Hovered;
	
	UFUNCTION()
	void AddConnection(EDirection Direction_, FString Name_);

	UFUNCTION()
	void MenuButtonOnHovered();

	UFUNCTION()
	void BindOnHovered();

	
};
