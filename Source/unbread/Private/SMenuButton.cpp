// Fill out your copyright notice in the Description page of Project Settings.


#include "SMenuButton.h"
#include "iostream"
#include "Components/Button.h"

void USMenuButton::AddConnection(EDirection Direction_, FString Name_)
{
	if(Connections.IsEmpty())
	{
		Connections.SetNum(6);
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


