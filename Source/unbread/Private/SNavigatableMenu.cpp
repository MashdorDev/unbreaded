// Fill out your copyright notice in the Description page of Project Settings.


#include "SNavigatableMenu.h"

#include "FrameTypes.h"
#include "PropertyAccess.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Framework/Application/NavigationConfig.h"

void USNavigatableMenu::AddButton(USMenuButton* Button)
{
	if(Buttons.Find(Button->Name)) return; // already exists

	Button->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if(UCanvasPanelSlot* I = UWidgetLayoutLibrary::SlotAsCanvasSlot(Button))
	{
		I->SetAlignment(FVector2D(0.5f, 0.5f));
	}
	
	// Custom Hover functionality, for mousing over
	Button->BindOnHovered();
	Button->Hovered.AddDynamic(this, &USNavigatableMenu::SetSelected);

	
	
	Buttons.Add(Button->Name, Button);
}

void USNavigatableMenu::Navigate(EDirection Direction)
{
	FString Name = Selected->Connections[Direction];
	
	if(Name == "") return;
	
	if(USMenuButton* Button = *Buttons.Find(Name)) SetSelected(Button);

}

void USNavigatableMenu::AddConnection(USMenuButton* FromButton, USMenuButton* ToButton, EDirection Direction)
{
	if(!Buttons.Find(FromButton->Name) || !Buttons.Find(ToButton->Name)) return;

	FromButton->AddConnection(Direction, ToButton->Name);
}


void USNavigatableMenu::SetSelected(USMenuButton* SelectedButton_)
{
	if (SelectedButton_)
	{
		Selected = *Buttons.Find(SelectedButton_->Name);

		if (Selected && Selected->Button)
		{
			const UCanvasPanelSlot* S = UWidgetLayoutLibrary::SlotAsCanvasSlot(Selected);
			UCanvasPanelSlot* I = UWidgetLayoutLibrary::SlotAsCanvasSlot(SelectedImage);

			if (!S || !I) return;
			
			if (Selected->Button->GetIsFocusable())
			{
				// Set focus on the button if it's focusable
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("focus"));

				Selected->Button->SetFocus();
			}
			
			ResetLerp(S->GetPosition());
			
		}
	}
}

void USNavigatableMenu::LerpImage()
{
	ImageLerpT += 0.1 * LerpSpeed;
	CurrentLocation = FMath::Lerp(OriginLocation, DestinationLocation, ImageLerpT);
	ImageLocation->SetPosition(CurrentLocation);
	if(ImageLerpT >= 1.0f)
	{
		ImageLocation->SetPosition(DestinationLocation);
	}
}

void USNavigatableMenu::ResetLerp(FVector2D DestinationLocation_)
{
	OriginLocation = ImageLocation->GetPosition();
	DestinationLocation = DestinationLocation_;
	ImageLerpT = 0.0f;
}


void USNavigatableMenu::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if(ImageLerpT < 1.0f) { LerpImage(); }
}


void USNavigatableMenu::NativePreConstruct()
{
	Super::NativePreConstruct();
	ImageLocation = UWidgetLayoutLibrary::SlotAsCanvasSlot(SelectedImage);
	OriginLocation = ImageLocation->GetPosition();

	const auto& SlateApplication = FSlateApplication::Get();
	auto& NavigationConfig = *SlateApplication.GetNavigationConfig();

	NavigationConfig.bAnalogNavigation = false;

}

void USNavigatableMenu::ToggleVisibility_Implementation(bool Visible)
{
	this->SetVisibility((Visible) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void USNavigatableMenu::SetParentWidget_Implementation(const TScriptInterface<ISMenuWidgetInterface>& Parent)
{
	if(Parent)
	{
		ParentWidget = Parent;
		
	}
}

void USNavigatableMenu::OpenParentWidget_Implementation()
{
	ISMenuWidgetInterface::Execute_ToggleVisibility(ParentWidget.GetObject(), true);
	
}




