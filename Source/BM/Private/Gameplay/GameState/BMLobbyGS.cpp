// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/GameState/BMLobbyGS.h"

ABMLobbyGS::ABMLobbyGS()
{
}


void ABMLobbyGS::Multi_GetMessage_Implementation(const FText& Message)
{
	OnGetMessage.Broadcast(Message);
}

