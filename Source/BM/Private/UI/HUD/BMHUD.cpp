// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/BMHUD.h"

UBMWidgetController* ABMHUD::GetOverlayController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UBMWidgetController>(this,OverlayControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

void ABMHUD::InitialOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	
}
