// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerHealthBar.h"

#include "Components/ProgressBar.h"

void USPlayerHealthBar::SetHealth(float CurrentHealth, float MaxHealth)
{
	if(!HealthBar) return;

	BarPercentage = CurrentHealth / MaxHealth;

	// calc final percentage
	float interval = 1.0f / NumOfImages;

	HealthBar->SetPercent(BarPercentage);
	

	for(int i = NumOfImages - 1; i >= 0; i--)
	{
		if(BarPercentage <= i * interval)
		{

			if(BarPercentage > (i-1) * interval)
			{
				// found correct range
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("INDEX: %i"), i));

				SetHealthFace(NumOfImages - i);
				return;
			}
		}
	}

	
}

void USPlayerHealthBar::SetShield(float CurrentShield, float MaxShield)
{
	ShieldPercentage = CurrentShield / MaxShield;
	ShieldBar->SetPercent(ShieldPercentage);
}


void USPlayerHealthBar::OnConstruct()
{
	BiteImageTransform = UWidgetLayoutLibrary::SlotAsCanvasSlot(BiteImageEnd);

	
	NumOfImages = Images.Num();
	CurImage = 0;
	SetHealthFace(0);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Construct health bar"));
	
}

void USPlayerHealthBar::SetHealthFace(const int& index)
{
	if(index >= Images.Num()) return;
	if(!Images[index]) return;
	HealthFace->SetBrushFromTexture(Images[index], true);
}
