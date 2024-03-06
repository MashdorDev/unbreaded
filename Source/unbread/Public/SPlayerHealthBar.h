// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "SPlayerHealthBar.generated.h"

/**
 * 
 */
class UProgressBar;
class UImage;

UCLASS(Blueprintable)
class UNBREAD_API USPlayerHealthBar : public UCanvasPanel
{
	GENERATED_BODY()


	UFUNCTION(BlueprintCallable)
	void SetHealth(float CurrentHealth, float MaxHealth);

	UFUNCTION(BlueprintCallable)
	void SetShield(float CurrentShield, float MaxShield);
	
	UFUNCTION(BlueprintCallable)
	void OnConstruct();

	UFUNCTION(BlueprintCallable)
	void SetHealthFace(const int& index);

	
	UPROPERTY(EditAnywhere, meta= (BindWidget))
	UProgressBar* HealthBar;
	
	UPROPERTY(EditAnywhere, meta= (BindWidget))
	UProgressBar* ShieldBar;

	UPROPERTY(EditAnywhere, meta= (BindWidget))
	UImage* HealthFace;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UImage* BiteImageEnd;
	
	UPROPERTY(EditAnywhere)
	TArray<UTexture2D*> Images;

	

	
private:
	float BarPercentage;
	float ShieldPercentage;
	int NumOfImages;
	int CurImage;

	UPROPERTY()
	UCanvasPanelSlot* BiteImageTransform;
	
	UPROPERTY()
	UCanvasPanelSlot* HealthBarTransform;

};
