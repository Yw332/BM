// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerState/BMPSLobby.h"

#include "Net/UnrealNetwork.h"

void ABMPSLobby::Server_SetPlayerName_Implementation(FName Name)
{
	PlayerName = Name;
}

void ABMPSLobby::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABMPSLobby, PlayerName);
}

void ABMPSLobby::SetPlayerName(const FName& Name)
{
	Server_SetPlayerName(Name);
	PlayerName = Name;
}
