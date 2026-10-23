// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Base/BMUserWidgetBase.h"

bool UBMUserWidgetBase::IsServer()
{
	UWorld* World = GetWorld();
	if (World == nullptr) return false;

	return World->GetNetMode() != ENetMode::NM_Client;
}

void UBMUserWidgetBase::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSetup();
}
