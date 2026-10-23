// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Login/MultiRoom.h"

#include "CreateSessionCallbackProxy.h"
#include "FindSessionsCallbackProxy.h"
#include "Setting/BMSettings.h"
#include "Misc/Parse.h"
#include "Components/EditableText.h"
#include "System/BMGlobalSubsystem.h"

void UMultiRoomWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	CreateRoomButton->OnClicked.AddDynamic(this, &ThisClass::CreateRoomButtonClicked);
	SearchRoomsButton->OnClicked.AddDynamic(this, &ThisClass::SearchRoomButtonClicked);

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		RetSessionsSubsystem = GameInstance->GetSubsystem<URetSessionsSubsystem>();
	}

	if (RetSessionsSubsystem)
	{
		RetSessionsSubsystem->RetOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateRoom);
		RetSessionsSubsystem->RetOnFindSessionsComplete.AddUObject(this, &ThisClass::OnSearchRooms);
	}
	
}

void UMultiRoomWidget::CreateRoomButtonClicked()
{
	CreateRoomButton->SetIsEnabled(false);

	UBMGlobalSubsystem* GlobalSubsystem = GetGameInstance()->GetSubsystem<UBMGlobalSubsystem>();
	GlobalSubsystem->PlayerCount = PlayerNum;
	
	if (MultiRoomType == EBMMultiRoomType::Steam && RetSessionsSubsystem->CanUse())
	{
		RetSessionsSubsystem->CreateSession(PlayerNum, Password);
		GlobalSubsystem->MultiRoomType = EBMMultiRoomType::Steam;

	}
	else
	{
		CreateLANRoom_BP();
		GlobalSubsystem->MultiRoomType = EBMMultiRoomType::LAN;
	}
}

void UMultiRoomWidget::SearchRoomButtonClicked()
{
	SearchRoomsButton->SetIsEnabled(false);

	if (MultiRoomType == EBMMultiRoomType::Steam && RetSessionsSubsystem->CanUse())
	{
		RetSessionsSubsystem->FindSessions(10000, Password);
	}
	else
	{
		SearchLANRoom_BP();
	}
}

void UMultiRoomWidget::OnCreateRoom(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		FString PathToLobby = UBMSettings::Get()->GetPathToLobby();
		PathToLobby = FString::Printf(TEXT("%s?listen"), *PathToLobby);
		UWorld* World = GetWorld();
		if (World)
		{
			World->ServerTravel(PathToLobby);
		}
		// UGameplayStatics::OpenLevel(this, FName("TestMap"), true, TEXT("listen"));
		GEngine->AddOnScreenDebugMessage(-1,15.f,FColor::Green,FString(TEXT("Session created successfully! Traveling to lobby: ") + PathToLobby));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1,15.f,FColor::Red,FString(TEXT("Failed to create session!")));
		CreateRoomButton->SetIsEnabled(true);
	}
}

void UMultiRoomWidget::OnSearchRooms(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful)
{
	if (RetSessionsSubsystem == nullptr) return;

	if (!bWasSuccessful)
	{
		GEngine->AddOnScreenDebugMessage( -1, 15.f, FColor::Red, FString(TEXT("Failed to find sessions!")) );
		SearchRoomsButton->SetIsEnabled(true);
		return;
	} 

	for (auto SessionResult : SessionResults)
	{
		FString SettingsValue;
		SessionResult.Session.SessionSettings.Get(FName("MatchType"), SettingsValue);
		if (SettingsValue == Password)
		{
			int32 PingInMs = SessionResult.PingInMs;
			int32 MaxPlayers = SessionResult.Session.SessionSettings.NumPublicConnections;
			int32 CurrentPlayers = MaxPlayers - SessionResult.Session.NumOpenPublicConnections;
			FString RoomName = SessionResult.Session.OwningUserName;

			TObjectPtr<URoomResult> RoomResult = CreateWidget<URoomResult>(GetOwningPlayer(), RoomResultClass);
			if (!IsValid(RoomResult)) continue;
			RoomResult->InitRoomResult(PingInMs, CurrentPlayers, MaxPlayers, RoomName);
			RoomResult->SetSearchResult(SessionResult);
			RoomListScrollBox->AddChild(RoomResult);
		}
	}
}
