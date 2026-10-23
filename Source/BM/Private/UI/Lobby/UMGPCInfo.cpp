// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/UMGPCInfo.h"
#include "GameFramework/PlayerState.h"
#include "RetSessionsSubsystem.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "UI/UIPreDefine.h"
#include "System/BMGlobalSubsystem.h"


void UUMGPCInfo::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUMGPCInfo::InitializeData(FName PlayerName, uint32 PlayerId)
{
	SetPlayerName(PlayerName);

	APlayerController* PC = GetOwningPlayer();
	if (!PC) return;
	
	APlayerState* PS = PC->GetPlayerState<APlayerState>();
	if (!PS) return;

	// UE_LOG(LogTemp, Warning, TEXT("PlayerUID: %d, TempPlayerUID: %d"), PlayerUID, TempPlayerUID);
	if (PlayerId == PS->GetPlayerId())
	{
		LogOutButton->SetIsEnabled(true);
		UE_LOG(LogTemp, Warning, TEXT("Button Enabled"));
	}
	else
	{
		LogOutButton->SetIsEnabled(false);
		UE_LOG(LogTemp, Warning, TEXT("Button Disabled"));
	}
	
	if (IsServer())
	{
		LogOutButton->SetIsEnabled(true);
		UE_LOG(LogTemp, Warning, TEXT("Button Enabled Due to In Server"));
	}
	LogOutButton->OnClicked.AddDynamic(this, &UUMGPCInfo::LogOut);
}

void UUMGPCInfo::SetLogOutButtonEnabled(bool bEnable)
{
	LogOutButton->SetIsEnabled(bEnable);
}

void UUMGPCInfo::LogOut()
{
	UWorld* World = GetWorld();
	if (!World) return;

	APlayerController* PC = GetOwningPlayer();
	if (!PC) return;
	
	ENetMode NetMode = World->GetNetMode();

	if (NetMode == NM_ListenServer)
	{
		const FString MainMapUrl = TEXT("/Game/Level/MainMap");

		for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
		{
			APlayerController* OtherPC = It->Get();
			if (!OtherPC) continue;

			if (!OtherPC->IsLocalController())
			{
				UE_LOG(LogTemp, Warning, TEXT("Kick Client: %s"), *OtherPC->GetName());
				OtherPC->ClientTravel(*MainMapUrl, TRAVEL_Absolute);
			}
		}	
	}
	
	FString Command = FString::Printf(TEXT("Open %s"), TEXT("MainMap"));
	PC->ConsoleCommand(*Command);
}

void UUMGPCInfo::SetPlayerName(FName PlayerName)
{
	PlayerNameText->SetText(FText::FromName(PlayerName));
}
