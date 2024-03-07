// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerHealthBar.h"

#include "Components/CanvasPanelSlot.h"
#include "Components/ProgressBar.h"

void USPlayerHealthBar::SetHealth(float CurrentHealth, float MaxHealth)
{
	if(!HealthBar) return;

	BarPercentage = CurrentHealth / MaxHealth;

	HealthBar->SetPercent(BarPercentage);

	// update the bite mark image position on the end of the progress bar
	float X = HealthBarTransform->GetPosition().X - HealthBarTransform->GetSize().X / 2;
	FVector2D T = FVector2D(X + (BarPercentage * HealthBarTransform->GetSize().X), BiteImageTransform->GetPosition().Y);
	BiteImageTransform->SetPosition(T);
	
	float interval = 1.0f / NumOfImages;
	// iterate through each image, and calculate which section the bar percentage is in.
	for(int i = NumOfImages - 1; i >= 0; i--)
	{
		// check upper limit
		if(BarPercentage <= i * interval)
		{
			// check lower limit
			if(BarPercentage > (i-1) * interval)
			{
				// found correct range
				// inverse the index, because the faces are in reverse order
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
	HealthBarTransform = UWidgetLayoutLibrary::SlotAsCanvasSlot(HealthBar);
	

	
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
