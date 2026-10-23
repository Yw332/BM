// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Base/BMUserWidgetBase.h"
#include "MainMenu.generated.h"

class UWidgetSwitcher;
class UButton;
/**
 * 
 */
UCLASS(Blueprintable)
class BM_API UMainMenu : public UBMUserWidgetBase
{
	GENERATED_BODY()

public:



protected:
	
	UPROPERTY(meta=(BindWidget), BlueprintReadWrite)
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;
	
#pragma region UMainMenu
	UPROPERTY(meta=(BindWidget), BlueprintReadWrite)
	TObjectPtr<UButton> SingleButton;

	UPROPERTY(meta=(BindWidget), BlueprintReadWrite)
	TObjectPtr<UButton> MultiButton;

	UPROPERTY(meta=(BindWidget), BlueprintReadWrite)
	TObjectPtr<UButton> SettingsButton;

	UPROPERTY(meta=(BindWidget), BlueprintReadWrite)
	TObjectPtr<UButton> DeveloperButton;
	
	UPROPERTY(meta=(BindWidget), BlueprintReadWrite)
	TObjectPtr<UButton> QuitButton;
#pragma endregion


};
