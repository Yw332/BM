// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/BMGameState.h"
#include "BMLobbyGS.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGetMessage, const FText&, Message);

/**
 * 
 */
UCLASS()
class BM_API ABMLobbyGS : public ABMGameState
{
	GENERATED_BODY()
	ABMLobbyGS();

public:
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Multi_GetMessage(const FText& Message);

	FOnGetMessage OnGetMessage;
	
};
