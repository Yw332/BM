// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BMBasePlayerController.h"
#include "BMPCLobby.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerChanged, const TArray<FName>&, PlayerNames,  const TArray<uint32>& , PlayerUIDs);

/**
 * 
 */
UCLASS()
class BM_API ABMPCLobby : public ABMBasePlayerController
{
	GENERATED_BODY()

public:

	UFUNCTION(Server, Reliable, BlueprintCallable)
	virtual void Server_SendMessage(const FText& Message);

	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION(NetMulticast, Reliable)
	void Multi_PlayerChanged(const TArray<FName>& PlayerNames, const TArray<uint32>& PlayerIds);
	
	FOnPlayerChanged OnPlayerChanged;
};
