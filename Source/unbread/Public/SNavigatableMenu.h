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
class UImage;

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
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void NativePreConstruct() override;

// MenuWidgetInterface overrides
	

private:
	float ImageLerpT;
	
	FVector2D DestinationLocation;
	FVector2D DestinationSize;

	FVector2D OriginLocation;
	FVector2D CurrentLocation;

	FVector2D OriginSize;
	FVector2D CurrentSize;




	void ResetLerp(FVector2D DestinationLocation_, FVector2D DestinationSize_);


};
