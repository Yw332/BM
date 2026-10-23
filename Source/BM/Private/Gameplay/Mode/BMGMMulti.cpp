// GameMode.cpp
#include "Gameplay/Mode/BMGMMulti.h"

#include "TimerManager.h"
#include "Engine/World.h"
#include "Gameplay/GameState/BMGSMulti.h"
#include "System/BMGlobalSubsystem.h"

ABMGMMulti::ABMGMMulti()
{
	RemainingPlayers = 0;  // 存活玩家初始为0
}

void ABMGMMulti::BeginPlay()
{
	Super::BeginPlay();
	SetGameState(EGameState::Preloading);

	WaitingTimerDel.BindUObject(this,&ThisClass::WaitingForPlayers);
	GetWorld()->GetTimerManager().SetTimerForNextTick(WaitingTimerDel);
}

void ABMGMMulti::WaitingForPlayers()
{
	if (CheckForAllPlayersInitialized())
	{
		StartReady();
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(WaitingTimerDel);
	}
}

bool ABMGMMulti::CheckForAllPlayersInitialized()
{
	if (bIsAllPlayersInitialized) return true;
	
	UBMGlobalSubsystem* Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<UBMGlobalSubsystem>();
	if (Subsystem->PlayerCount == GetWorld()->GetNumPlayerControllers())
	{
		bIsAllPlayersInitialized = true;
	}

	return bIsAllPlayersInitialized;
}

void ABMGMMulti::StartReady()
{
	SetGameState(EGameState::Readying);
	RemainingPlayers = GetWorld()->GetNumPlayerControllers();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::StartGame, ReadyTime, false);
}

void ABMGMMulti::StartGame()
{
	SetGameState(EGameState::InGame);
	
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,this, &ThisClass::CheckVictoryConditions, MaxGameTime, false);
}

void ABMGMMulti::CheckVictoryConditions()
{
	APlayerController* PC = nullptr;
	
	if (RemainingPlayers > 1)
	{
		 if (GetWorld()->GetTimerManager().GetTimerRemaining(TimerHandle) > 0 )
		 {
			 return; // Game continue
		 }
		 else
		 {
			 EndGame(PC); // Game time over
		 }	
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		EndGame(PC); 
	}
	
}

void ABMGMMulti::EndGame(APlayerController* PC)
{
	// 游戏结束，切换状态为GameOver
	SetGameState(EGameState::GameOver);
}

void ABMGMMulti::SetGameState(EGameState NewState)
{
	ABMGSMulti* GS = GetGameState<ABMGSMulti>();
	if (GS)
	{
		GS->SetGameState(NewState);
	}

	OnGameStateChanged.Broadcast(NewState);
}

void ABMGMMulti::OnPlayerDied()
{
	RemainingPlayers--;
	CheckVictoryConditions();
}