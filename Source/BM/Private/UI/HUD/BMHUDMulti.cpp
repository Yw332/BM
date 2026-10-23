// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/BMHUDMulti.h"
#include "Gameplay/GameState/BMGSMulti.h"
#include "Player/PlayerState/BMPlayerState.h"

void ABMHUDMulti::BeginPlay()
{
	Super::BeginPlay();
}



void ABMHUDMulti::InitialOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC,
	UAttributeSet* AS)
{
	if (OverlayWidget) return;
	
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class is not set up, please set up in BP_AuraHUD"))

	OverlayWidget = CreateWidget<UBMUIIGMain>(GetWorld(),OverlayWidgetClass);

	const FWidgetControllerParams WCParams(PC,PS,ASC,AS);
	UBMUIMultiOverlayController* LocalOverlayController = Cast<UBMUIMultiOverlayController>(GetOverlayController(WCParams));

	OverlayWidget->SetWidgetController(LocalOverlayController);
	LocalOverlayController->BroadcastInitialValues();

	OverlayWidget->AddToViewport();
	
	if (ABMGSMulti* GS = GetWorld()->GetGameState<ABMGSMulti>())
	{
		GS->OnGameStateChanged.AddDynamic(this, &ThisClass::HandleGameStateChanged);
	}

	ABMPlayerState* LocalPlayerState = Cast<ABMPlayerState>(PS);
	LocalPlayerState->OnPlayerStateDeath.AddLambda([this](){
		
		Cast<UBMUIIGMain>(OverlayWidget)->SwitchState(EGameState::PlayerDead);
	});
	
}

void ABMHUDMulti::HandleGameStateChanged(EGameState NewState)
{
	Cast<UBMUIIGMain>(OverlayWidget)->SwitchState(NewState);
}
