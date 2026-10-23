// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/GameState/BMGSMulti.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Gameplay/Mode/BMGMMulti.h"

ABMGSMulti::ABMGSMulti()
{
	// Game state does not tick by default but for our game we would like it to. 
	PrimaryActorTick.bCanEverTick = true;
}

void ABMGSMulti::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABMGSMulti, CurrentGameState);
}


void ABMGSMulti::SetGameState(EGameState NewState)
{
	if (!HasAuthority())
	{
		// 状态只能在服务器改
		return;
	}

	CurrentGameState = NewState;

	// 服务器本地也需要响应一次
	OnRep_GameState();
}

void ABMGSMulti::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()){
		
		if (ABMGMMulti* GM = GetWorld()->GetAuthGameMode<ABMGMMulti>())
		{
			GM->OnGameStateChanged.AddDynamic(this,	&ABMGSMulti::HandleGameModeGameStateChanged);
		}
	}	
}

void ABMGSMulti::OnRep_GameState()
{
	OnGameStateChanged.Broadcast(CurrentGameState);
}

void ABMGSMulti::HandleGameModeGameStateChanged(EGameState NewState)
{
	// 这里通常不会仅仅赋值，而是走一个 SetGameState，让它负责复制和广播
	SetGameState(NewState);
}
