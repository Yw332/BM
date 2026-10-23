// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/Base/BMUserWidgetBase.h"
#include "UI/WidgetController/BMWidgetController.h"
#include "BMHUD.generated.h"

/**
 * 
 */
UCLASS()
class BM_API ABMHUD : public AHUD
{
	GENERATED_BODY()

public:

	
	UFUNCTION(BlueprintCallable)
	UBMWidgetController* GetOverlayController(const FWidgetControllerParams& WCParams);

	
	UFUNCTION(BlueprintCallable)
	virtual void InitialOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

protected:
	
	UPROPERTY()
	TObjectPtr<UBMUserWidgetBase> OverlayWidget;

	UPROPERTY()
	TObjectPtr<UBMWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UBMUserWidgetBase> OverlayWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UBMWidgetController> OverlayControllerClass;
};
