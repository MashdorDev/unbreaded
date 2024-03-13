// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SMenuWidgetInterface.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Slider.h"
#include "SMenuButton.generated.h"

/**
 * 
 */
class UButton;

UENUM()
enum EDirection { Up = 0, Right, Down, Left, In, Out };


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMenuButtonHovered, USMenuButton*, Button);

UCLASS(Blueprintable)
class UNBREAD_API USMenuButton : public UCanvasPanel
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UButton* Button;
	
	UPROPERTY(EditAnywhere)
	TArray<FString> Connections;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCanvasPanel* ChildCanvas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCanvasPanel* ParentCanvas;
	
	UPROPERTY(EditAnywhere)
	FString Name;

	UPROPERTY(EditAnywhere)
	bool IsPermanent;

	UPROPERTY(BlueprintAssignable)
	FOnMenuButtonHovered Hovered;
	
	UFUNCTION()
	void AddConnection(EDirection Direction_, FString Name_);

	UFUNCTION()
	void MenuButtonOnHovered();

	UFUNCTION()
	void BindOnHovered();

	UFUNCTION()
	bool HasChildCanvas() const { return (ChildCanvas) ? true : false;}

	UFUNCTION()
	bool HasParentCanvas() const { return (ParentCanvas) ? true : false;}

// SMenuWidgetInterface 
	



};
