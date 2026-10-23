// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetSwitcher.h"
#include "UI/Base/BMUserWidgetBase.h"
#include "BMUIIGMain.generated.h"

enum class EGameState : uint8;
/**
 * 
 */
UCLASS()
class BM_API UBMUIIGMain : public UBMUserWidgetBase
{
	GENERATED_BODY()

public:
	
	void SwitchState(EGameState NewState);

protected:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher> StageUISwitcher;

};
