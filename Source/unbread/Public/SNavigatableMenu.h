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
class UNBREAD_API USNavigatableMenu : public UUserWidget, public ISMenuWidgetInterface
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	TMap<FString, USMenuButton*> Buttons;

	UPROPERTY(EditAnywhere)
	USMenuButton* Selected;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UImage* SelectedImage;

	UPROPERTY(EditAnywhere)
	FVector2D SelectedImagePadding;

	UPROPERTY()
	UCanvasPanelSlot* ImageLocation;

	UPROPERTY(EditAnywhere)
	float LerpSpeed;
	
	UFUNCTION(BlueprintCallable)
	void AddButton(USMenuButton* Button);

	UFUNCTION(BlueprintCallable)
	void Navigate(EDirection Direction);

	UFUNCTION(BlueprintCallable)
	void AddConnection(USMenuButton* FromButton, USMenuButton* ToButton, EDirection Direction);

	UFUNCTION(BlueprintCallable)
	void SetSelected(USMenuButton* Selected_);

	UFUNCTION(BlueprintCallable)
	USMenuButton* GetSelected(){return Selected;}

	UFUNCTION()
	void LerpImage();
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void NativePreConstruct() override;

// MenuWidgetInterface overrides
	void ToggleVisibility_Implementation(bool Visible) override;
	void SetParentWidget_Implementation(const TScriptInterface<ISMenuWidgetInterface>& Parent) override;
	void OpenParentWidget_Implementation() override;

private:
	float ImageLerpT;
	FVector2D DestinationLocation;
	FVector2D OriginLocation;
	FVector2D CurrentLocation;

	TScriptInterface<ISMenuWidgetInterface> ParentWidget;

	void ResetLerp(FVector2D DestinationLocation_);


};
