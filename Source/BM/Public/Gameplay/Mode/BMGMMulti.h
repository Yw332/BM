// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/BMGameMode.h"
#include "Gameplay/BMGamePreDefine.h"
#include "BMGMMulti.generated.h"


class URetSessionsSubsystem;
class ABMPlayerState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGMGameStateChangedSignature, EGameState, NewState);

/**
 * 
 */
UCLASS()
class BM_API ABMGMMulti : public ABMGameMode
{
	GENERATED_BODY()

public:
	ABMGMMulti();

	
	void OnPlayerDied();

	UPROPERTY(BlueprintAssignable)
	FOnGMGameStateChangedSignature OnGameStateChanged;
	
protected:
	virtual void BeginPlay() override;
	virtual void WaitingForPlayers();
	virtual bool CheckForAllPlayersInitialized();
	
	void StartReady();
	void StartGame();
	void EndGame(APlayerController* PC);

	void CheckVictoryConditions();
	void SetGameState(EGameState NewState);

	
protected:
	UPROPERTY(EditAnywhere)
	float MaxGameTime = 100;
	UPROPERTY(EditAnywhere)
	float ReadyTime = 15;
	
	int32 RemainingPlayers;
	FTimerDelegate WaitingTimerDel;

	FTimerHandle TimerHandle;
	

private:
	bool bIsAllPlayersInitialized = false;
	
};
