// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Login/RoomResult.h"
#include "Components/TextBlock.h"
#include "JoinSessionCallbackProxy.h"
#include "OnlineSubsystemUtils.h"
#include "System/BMGlobalSubsystem.h"
class UTextBlock;

void URoomResult::NativeConstruct()
{
	Super::NativeConstruct();

	JoinRoomButton->OnClicked.AddDynamic(this, &ThisClass::JoinRoomButtonClicked);

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		RetSessionsSubsystem = GameInstance->GetSubsystem<URetSessionsSubsystem>();
	}

	if (RetSessionsSubsystem)
	{
		RetSessionsSubsystem->RetOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinRoom);
	}
}

void URoomResult::InitRoomResult(float InPingInMs, int32 InCurrentPlayers, int32 InMaxPlayers, const FString& InRoomName)
{
	PingInMs = InPingInMs;
	CurrentPlayers = InCurrentPlayers;
	MaxPlayers = InMaxPlayers;
	RoomName = InRoomName;
	PingTextBlock->SetText(FText::FromString(FString::Printf(TEXT("帧率: %.0fms "), InPingInMs)));
	PlayersTextBlock->SetText(FText::FromString(FString::Printf(TEXT("  玩家: %d/%d "), InCurrentPlayers, InMaxPlayers)));
	RoomNameTextBlock->SetText(FText::FromString(FString::Printf(TEXT("  房间: %s" ), *InRoomName)));
}

void URoomResult::JoinRoomButtonClicked()
{
	if (RetSessionsSubsystem == nullptr) return;

	GEngine->AddOnScreenDebugMessage( -1, 15.f, FColor::Green, FString::Printf(TEXT("Ready to Joining room: %s"), *RoomName));


	if (GetGameInstance()->GetSubsystem<UBMGlobalSubsystem>()->MultiRoomType == EBMMultiRoomType::Steam)
	{
		RetSessionsSubsystem->JoinSession(SearchResult);
	}else
	{
		JoinRoom_BP();
	}
}

void URoomResult::OnJoinRoom(EOnJoinSessionCompleteResult::Type Result)
{
	if (Result != EOnJoinSessionCompleteResult::Success)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Failed to join session: %s"), *RoomName));
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Error code: %d"), static_cast<int32>(Result)));
		}
		return;
	}
	
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld(), FName("STEAM"));

	if (Subsystem)
	{
		IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			FString Address;
			SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("Joining room at address: %s"), *Address));
			APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
			if (PlayerController)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("Traveling to address: %s"), *Address));
				PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
			}
		}
	}
}
