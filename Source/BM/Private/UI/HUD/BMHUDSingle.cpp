// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/BMHUDSingle.h"

void ABMHUDSingle::InitialOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC,
	UAttributeSet* AS)
{
	Super::InitialOverlay(PC, PS, ASC, AS);

	if (OverlayWidget) return;
	
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class is not set up, please set up in BP_AuraHUD"))

	OverlayWidget = CreateWidget<UBMUserWidgetBase>(GetWorld(),OverlayWidgetClass);

	const FWidgetControllerParams WCParams(PC,PS,ASC,AS);
	UBMWidgetController* LocalOverlayController = GetOverlayController(WCParams);

	OverlayWidget->SetWidgetController(LocalOverlayController);
	LocalOverlayController->BroadcastInitialValues();

	OverlayWidget->AddToViewport();
}
