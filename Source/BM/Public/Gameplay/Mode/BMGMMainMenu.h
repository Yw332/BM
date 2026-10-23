// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/BMGameMode.h"
#include "BMGMMainMenu.generated.h"

/**
 * 
 */
UCLASS()
class BM_API ABMGMMainMenu : public ABMGameMode
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	void DestroyeSession();
	UFUNCTION(BlueprintImplementableEvent)
	void DestroySession_BP();
};
