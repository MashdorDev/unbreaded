// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "SMenuButton.h"
#include "Widgets/SWidget.h"
#include "ButtonWidgetStyle.generated.h"
#include "SNavigatableMenu.generated.h"

/**
 * 
 */
class UButton;
class UImage;
//class SWidget;

UCLASS()
class UNBREAD_API USNavigatableMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TMap<FString, USMenuButton*> Buttons;

	UPROPERTY(EditAnywhere)
	USMenuButton* Selected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UCanvasPanel* TopMostCanvas;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UImage* SelectedImage;

	UPROPERTY(EditAnywhere)
	FVector2D SelectedImagePadding;

	UPROPERTY()
	UCanvasPanelSlot* ImageTransform;

	UPROPERTY(EditAnywhere)
	float LerpSpeed;
	
	UFUNCTION(BlueprintCallable)
	void AddButton(USMenuButton* Button);

	UFUNCTION(BlueprintCallable)
	void Navigate(EDirection Direction);

	UFUNCTION(BlueprintCallable)
	void AddConnection(USMenuButton* FromButton, USMenuButton* ToButton, EDirection Direction, bool TwoWay);

	UFUNCTION(BlueprintCallable)
	void SetSelected(USMenuButton* Selected_);

	UFUNCTION()
	void LerpImage();

	UFUNCTION(BlueprintCallable)
	void CloseAllOpenCanvas();
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void NativePreConstruct() override;

// MenuWidgetInterface overrides
	

private:
	float ImageLerpT;
	
	FVector2D DestinationLocation;
	FVector2D OriginLocation;
	FVector2D CurrentLocation;

	float DestinationRotation;
	float OriginRotation;
	float CurrentRotation;

	UPROPERTY()
	TArray<UCanvasPanel*> OpenCanvas;

	UPROPERTY()
	UButton* ActiveTab;
	
	void ResetLerp(FVector2D DestinationLocation_, float DestinationRotation_);


};
