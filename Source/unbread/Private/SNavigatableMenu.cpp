// Fill out your copyright notice in the Description page of Project Settings.


#include "SNavigatableMenu.h"

#include "FrameTypes.h"
#include "PropertyAccess.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Framework/Application/NavigationConfig.h"
#include "Widgets/SViewport.h"

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
	if(!Selected) return;
	FString Name = Selected->Connections[Direction];
	
	if(Name == "") return;
	
	if(USMenuButton* Button = *Buttons.Find(Name))
	{
		
		if(Selected->HasChildrenCanvas() && Direction == EDirection::In)
		{
			if(!Selected->IsPermanent)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Not permanent"));
				Button->ParentCanvas->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				// player selected permanent tab, therefore close all open canvas
				CloseAllOpenCanvas();
				Selected->Button->SetIsEnabled(false);
				if(ActiveTab) ActiveTab->SetIsEnabled(true);
				ActiveTab = Selected->Button;
			}
			
			// TODO find a better way to do this -
			// right now hard coded to only disable the first child. maybe split children into cosmetic and functional?
			// or make canvas wrapper class with flag to say whether is should be permanent or not
			OpenCanvas.Push(Selected->ChildCanvas[0]);
			for(UCanvasPanel* C : Selected->ChildCanvas)
			{
				C->SetVisibility(ESlateVisibility::Visible);
				C->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("enabling child"));
			

		}
		else if(Selected->HasParentCanvas() && Direction == EDirection::Out)
		{
			Selected->ParentCanvas->SetVisibility(ESlateVisibility::Visible);
			Selected->ParentCanvas->SetVisibility((ESlateVisibility::SelfHitTestInvisible));

			for(UCanvasPanel* C : Button->ChildCanvas)
			{
				C->SetVisibility(ESlateVisibility::Collapsed);
			}
			/*Button->ChildCanvas->SetVisibility(ESlateVisibility::Collapsed);*/
		}
		SetSelected(Button);
		
	}
	

}

void USNavigatableMenu::AddConnection(USMenuButton* FromButton, USMenuButton* ToButton, EDirection Direction, bool TwoWay)
{
	if(!Buttons.Find(FromButton->Name) || !Buttons.Find(ToButton->Name)) return;

	FromButton->AddConnection(Direction, ToButton->Name);

	// add another connection in the opposite direction
	if(TwoWay)
	{
		switch(Direction)
		{
		case EDirection::Up:
			ToButton->AddConnection(EDirection::Down, FromButton->Name);
		case EDirection::Down:
			ToButton->AddConnection(EDirection::Up, FromButton->Name);
		case EDirection::Left:
			ToButton->AddConnection(EDirection::Right, FromButton->Name);
		case EDirection::Right:
			ToButton->AddConnection(EDirection::Left, FromButton->Name);
		
		default:
			break;
		}
	}
	
	
}


void USNavigatableMenu::SetSelected(USMenuButton* SelectedButton_)
{
	if (SelectedButton_)
	{
		Selected = *Buttons.Find(SelectedButton_->Name);

		if (Selected && Selected->Button)
		{
			const UCanvasPanelSlot* S = UWidgetLayoutLibrary::SlotAsCanvasSlot(Selected);
			
			
			if (!S) return;
			
			if (Selected->Button->GetIsFocusable())
			{
				// Set focus on the button if it's focusable
				Selected->Button->SetFocus();
				
			}
			if(Selected->IsPermanent)
			{
				if(ActiveTab) ActiveTab->SetIsEnabled(true);
				Selected->Button->SetIsEnabled(true);
				ActiveTab = nullptr;
			}

			ResetLerp(S->GetPosition() + Selected->SelectedLocationOffset, Selected->GetRenderTransform().Angle + Selected->SelectedRotationOffset);
			
		}
	}
}

void USNavigatableMenu::LerpImage()
{
	ImageLerpT += 0.1 * LerpSpeed;
	CurrentLocation = FMath::Lerp(OriginLocation, DestinationLocation, ImageLerpT);
	CurrentRotation = FMath::Lerp(OriginRotation, DestinationRotation, ImageLerpT);

	ImageTransform->SetPosition(CurrentLocation);
	SelectedImage->SetRenderTransformAngle(CurrentRotation);

	
	if(ImageLerpT >= 1.0f)
	{
		SelectedImage->SetRenderTransformAngle(DestinationRotation);
		ImageTransform->SetPosition(DestinationLocation);
	}
}

void USNavigatableMenu::CloseAllOpenCanvas()
{
	for(auto& C : OpenCanvas)
	{
		C->SetVisibility(ESlateVisibility::Collapsed);
	}
	OpenCanvas.Empty();
}

void USNavigatableMenu::ResetLerp(FVector2D DestinationLocation_, float DestinationRotation_)
{
	OriginLocation = ImageTransform->GetPosition();
	DestinationLocation = DestinationLocation_;

	OriginRotation = SelectedImage->GetRenderTransform().Angle;
	DestinationRotation = DestinationRotation_;
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
	ImageTransform = UWidgetLayoutLibrary::SlotAsCanvasSlot(SelectedImage);
	OriginLocation = ImageTransform->GetPosition();

	const auto& SlateApplication = FSlateApplication::Get();
	auto& NavigationConfig = *SlateApplication.GetNavigationConfig();

	NavigationConfig.bAnalogNavigation = false;
	NavigationConfig.bKeyNavigation = false;
	NavigationConfig.bTabNavigation = false;

	if(SelectedImagePadding.Length() <= 0.0f)
	{
		SelectedImagePadding.X = 20.0f;
		SelectedImagePadding.Y = 20.0f;

	}

}








