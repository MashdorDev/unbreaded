// Fill out your copyright notice in the Description page of Project Settings.


#include "SNavigatableMenu.h"

#include "Components/Image.h"

void USNavigatableMenu::AddButton(USMenuButton* Button)
{
	if(Buttons.Find(Button->Name))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Button Already Exists!"));
		return;
	}
	

	Buttons.Add(Button->Name, Button);
}

void USNavigatableMenu::Navigate(EDirection Direction)
{
	
	FString Name = Selected->Connections[Direction];
	if(Name == "")
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
        				TEXT("No Connection in desired direction"));
		
		return;		

	}
	if(USMenuButton* Button = *Buttons.Find(Name))
     	{
     		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
     			TEXT("Navigating to button") + Name);
     		SetSelected(Button);
     	}

	// This should not have taken this long
}

void USNavigatableMenu::AddConnection(USMenuButton* FromButton, USMenuButton* ToButton, EDirection Direction)
{
	

	
	if(!Buttons.Find(FromButton->Name) || !Buttons.Find(ToButton->Name))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
				TEXT("Unable to add connection"));
		return;
	}

	FromButton->AddConnection(Direction, ToButton->Name);

	
}


void USNavigatableMenu::SetSelected(USMenuButton* SelectedButton_)
{
	if(Selected)
	{
		Selected->Button->SetStyle(SelectedButton_->Button->GetStyle());
	}

	
	Selected = *Buttons.Find(SelectedButton_->Name);
	FButtonStyle ButtonStyle = Selected->Button->GetStyle();
	ButtonStyle.Normal = ButtonStyle.Hovered;
	
	Selected->Button->SetStyle(ButtonStyle);
	
}
