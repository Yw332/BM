// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Controller/BMPCLobby.h"

#include "Gameplay/GameState/BMLobbyGS.h"

void ABMPCLobby::Server_SendMessage_Implementation(const FText& Message)
{
	if (Message.IsEmpty()) return;

	ABMLobbyGS* GS = GetWorld()->GetGameState<ABMLobbyGS>();
	GS->Multi_GetMessage(Message);
}

void ABMPCLobby::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	bShowMouseCursor = true;
	bEnableClickEvents = true;
	
	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
	SetInputMode(InputMode);
}

void ABMPCLobby::Multi_PlayerChanged_Implementation(const TArray<FName>& PlayerNames, const TArray<uint32>& PlayerIds)
{
	OnPlayerChanged.Broadcast(PlayerNames, PlayerIds);
}