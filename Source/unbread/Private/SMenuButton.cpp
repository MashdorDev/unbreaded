// Fill out your copyright notice in the Description page of Project Settings.


#include "SMenuButton.h"
#include "iostream"
#include "Components/Button.h"

void USMenuButton::AddConnection(EDirection Direction_, FString Name_)
{
	if(Connections.IsEmpty())
	{
		Connections.SetNum(4);
	}
	Connections[Direction_] = Name_;
}

void USMenuButton::MenuButtonOnHovered()
{
	Hovered.Broadcast(this);

}

void USMenuButton::BindOnHovered()
{
	Button->OnHovered.AddDynamic(this, &USMenuButton::MenuButtonOnHovered);
}

void USMenuButton::SetChildWidget_Implementation(const TScriptInterface<ISMenuWidgetInterface>& Child)
{

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("widget child imp"));
	if(Child)
	{
		HasChild = true;
		ChildWidget = Child;
	}
}

void USMenuButton::OpenChildWidget_Implementation()
{
	ISMenuWidgetInterface::Execute_ToggleVisibility(ChildWidget.GetObject(), true);
	
}

void USMenuButton::CloseChildWidget_Implementation()
{
	ChildWidget->Execute_ToggleVisibility(this, false);



}


