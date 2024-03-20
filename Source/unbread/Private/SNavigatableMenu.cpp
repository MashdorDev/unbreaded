// Fill out your copyright notice in the Description page of Project Settings.


#include "SNavigatableMenu.h"

#include "FrameTypes.h"
#include "PropertyAccess.h"
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
		
		if(Selected->HasChildCanvas() && Direction == EDirection::In)
		{
			if(!Selected->IsPermanent)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Not permanent"));
				Button->ParentCanvas->SetVisibility(ESlateVisibility::Collapsed);
			}
			
			Selected->ChildCanvas->SetVisibility(ESlateVisibility::Visible);
			Selected->ChildCanvas->SetVisibility((ESlateVisibility::SelfHitTestInvisible));
		}
		else if(Selected->HasParentCanvas() && Direction == EDirection::Out)
		{
			Selected->ParentCanvas->SetVisibility(ESlateVisibility::Visible);
			Selected->ParentCanvas->SetVisibility((ESlateVisibility::SelfHitTestInvisible));

			Button->ChildCanvas->SetVisibility(ESlateVisibility::Collapsed);
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
			const UCanvasPanelSlot* I = UWidgetLayoutLibrary::SlotAsCanvasSlot(SelectedImage);
			const UCanvasPanelSlot* B = UWidgetLayoutLibrary::SlotAsCanvasSlot(Selected->Button);
			
			if (!S || !I || !B) return;
			
			if (Selected->Button->GetIsFocusable())
			{
				// Set focus on the button if it's focusable
				Selected->Button->SetFocus();
				
			}
			
			FVector2D pos = Selected->GetRenderTransform().Translation;
			//pos = GetAbsolutePosOfChild(Selected);
		
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Position: %f %f"), pos.X, pos.Y));
			ResetLerp(S->GetPosition(), B->GetSize());
			
		}
	}
}

FVector2D USNavigatableMenu::GetAbsolutePosOfChild(UWidget* Child)
{
	FVector2D AbsolutePosition(0, 0);

	UWidget* CurrentWidget = Child;
	
	while(CurrentWidget->GetParent())
	{
		/*if(!CurrentWidget->GetParent())
		{
			break;
		}*/
		UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(CurrentWidget);
		FVector2D Anchor = CanvasSlot->GetAnchors().Minimum;
		FVector2D size = TopMostCanvas->GetDesiredSize();
		//FGeometry WidgetGeometry =  CurrentWidget->GetCachedGeometry();
		AbsolutePosition += CanvasSlot->GetPosition() + (CanvasSlot->GetAnchors().Minimum * size);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, CurrentWidget->GetName() + FString::Printf(TEXT("size: %f %f"), Anchor.X,  Anchor.Y));

		CurrentWidget = CurrentWidget->GetParent();
	}
	return FVector2D(AbsolutePosition.X, AbsolutePosition.Y);

}

void USNavigatableMenu::LerpImage()
{
	ImageLerpT += 0.1 * LerpSpeed;
	CurrentLocation = FMath::Lerp(OriginLocation, DestinationLocation, ImageLerpT);
	CurrentSize = FMath::Lerp(OriginSize, DestinationSize, ImageLerpT);
	ImageTransform->SetPosition(CurrentLocation);
	//ImageTransform->SetSize(CurrentSize);
	if(ImageLerpT >= 1.0f)
	{
		ImageTransform->SetPosition(DestinationLocation);
	}
}

void USNavigatableMenu::ResetLerp(FVector2D DestinationLocation_, FVector2D DestinationSize_)
{

	
	OriginLocation = ImageTransform->GetPosition();
	DestinationLocation = DestinationLocation_;
	
	OriginSize = ImageTransform->GetSize();
	DestinationSize = DestinationSize_ + SelectedImagePadding;
	
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








