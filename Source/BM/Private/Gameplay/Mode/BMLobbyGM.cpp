// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Mode/BMLobbyGM.h"
#include "Player/Controller/BMPCLobby.h"
#include "Player/PlayerState/BMPSLobby.h"

void ABMLobbyGM::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	APlayerController* NewPC = Cast<APlayerController>(NewPlayer);
	PCArray.AddUnique(NewPC);

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ABMLobbyGM::PCOnChanged, 0.1f, false);
	PCOnChanged();
}

void ABMLobbyGM::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	APlayerController* ExitingPC = Cast<APlayerController>(Exiting);
	PCArray.Remove(ExitingPC);

	PCOnChanged();
}

void ABMLobbyGM::StartGame()
{
	FString PathToGame = FString::Printf(TEXT("%s?listen"), *GameMapPath);
	GetWorld()->ServerTravel(PathToGame);
}

void ABMLobbyGM::PCOnChanged()
{
	TArray<FName> PlayerNames;
	TArray<uint32> PlayerIds;

	for (auto PCTemp : PCArray)
	{
		if (!PCTemp.IsValid()) return;

		ABMPCLobby* PC = Cast<ABMPCLobby>(PCTemp);
		ABMPSLobby* PS = PC->GetPlayerState<ABMPSLobby>();
		
		PlayerNames.Add(PS->GetPlayerName());
		PlayerIds.Add(PS->GetPlayerId());
	}

	for (auto PCTemp : PCArray)
	{
		if (!PCTemp.IsValid()) return;

		ABMPCLobby* PC = Cast<ABMPCLobby>(PCTemp);
		PC->Multi_PlayerChanged(PlayerNames, PlayerIds);
	}
	
}
