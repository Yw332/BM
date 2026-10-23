// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/BMWidgetController.h"

void UBMWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UBMWidgetController::BroadcastInitialValues()
{
	
}

void UBMWidgetController::BindCallbacksToDependencies()
{
	
}
