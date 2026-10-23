// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BMPlayerState.h"
#include "BMPSLobby.generated.h"

/**
 * 
 */
UCLASS()
class BM_API ABMPSLobby : public APlayerState
{
	GENERATED_BODY()

public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetPlayerName(const FName& Name);
	FName GetPlayerName() const { return PlayerName; }
	
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Player")
	void Server_SetPlayerName(FName Name);
	
protected:

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
	FName PlayerName = "Test";
	
};
