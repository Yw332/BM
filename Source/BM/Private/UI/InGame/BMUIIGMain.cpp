// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGame/BMUIIGMain.h"

#include "Gameplay/BMGamePreDefine.h"

void UBMUIIGMain::SwitchState(EGameState NewState)
{
	if (NewState == EGameState::Readying)
	{
		StageUISwitcher->SetActiveWidgetIndex(0);
	}
	else if (NewState == EGameState::InGame)
	{
		StageUISwitcher->SetActiveWidgetIndex(1);
		UBMUserWidgetBase* Widget = Cast<UBMUserWidgetBase>(StageUISwitcher->GetActiveWidget());

		if (Widget)
		{
			Widget->SetWidgetController(WidgetController);
		}
	}
	else if (NewState == EGameState::GameOver)
	{
		StageUISwitcher->SetActiveWidgetIndex(2);
	}
	else if (NewState == EGameState::PlayerDead)
	{
		StageUISwitcher->SetActiveWidgetIndex(3);
	}
}