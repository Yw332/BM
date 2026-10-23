// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BMHUD.h"
#include "UI/UIPreDefine.h"
#include "UI/InGame/BMUIIGMain.h"
#include "UI/WidgetController/BMUIMultiOverlayController.h"
#include "BMHUDMulti.generated.h"

/**
 * 
 */
UCLASS()
class BM_API ABMHUDMulti : public ABMHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void InitialOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	
	
protected:

	
	UFUNCTION()
	virtual void HandleGameStateChanged(EGameState NewState);
	
};
