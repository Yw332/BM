// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Login/LobbyMenu.h"
#include "Components/VerticalBox.h"
#include "Gameplay/Mode/BMLobbyGM.h"
#include "Kismet/GameplayStatics.h"
#include "System/BMGlobalSubsystem.h"
#include "UI/Lobby/UMGPCInfo.h"
#include "UI/Chat/ChatMessage.h"
#include "Components/Button.h"
#include "Gameplay/GameState/BMLobbyGS.h"
#include "Player/Controller/BMPCLobby.h"

void ULobbyMenu::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void ULobbyMenu::NativeConstruct()
{
	Super::NativeConstruct();
	
	ABMPCLobby* OwningPlayer = Cast<ABMPCLobby>(GetOwningPlayer());
	if (IsValid(OwningPlayer))
	{
		OwningPlayer->OnPlayerChanged.AddDynamic(this, &ThisClass::UpdatePlayerList);
	}

	ABMLobbyGS* GS = GetWorld()->GetGameState<ABMLobbyGS>();
	if (IsValid(GS))
	{
		GS->OnGetMessage.AddDynamic(this, &ThisClass::AddNewMessage);
	}
	
	StartGameButton->OnClicked.AddDynamic(this, &ThisClass::StartGame);
	StartGameButton->SetIsEnabled(false);
}

void ULobbyMenu::OnPlayerChanged(int32 PlayerCount)
{
	if (!IsServer()) return;
	
	UBMGlobalSubsystem* GlobalSubsystem = GetGameInstance()->GetSubsystem<UBMGlobalSubsystem>();
	if (GlobalSubsystem == nullptr) return;
	
	if (PlayerCount >= GlobalSubsystem->PlayerCount)
	{
		StartGameButton->SetIsEnabled(true);
	}
	else
	{
		StartGameButton->SetIsEnabled(false);
	}
}

void ULobbyMenu::StartGame()
{
	if (GameMode == nullptr)
	{
		GameMode = Cast<ABMLobbyGM>(UGameplayStatics::GetGameMode(this));
		if (GameMode == nullptr) return;
	};
	GameMode->StartGame();
}

void ULobbyMenu::UpdatePlayerList(const TArray<FName>& PlayerNames, const TArray<uint32>& PlayerIds)
{
	if (GetWorld()->bIsTearingDown) return;
	
	PlayerListVerticalBox->ClearChildren();

	for (int32 i = 0; i < PlayerNames.Num(); i++)
	{
		TObjectPtr<UUMGPCInfo> ChildWidget = CreateWidget<UUMGPCInfo>(GetOwningPlayer(), PlayerListClass);
		if (!IsValid(ChildWidget)) return;
		
		ChildWidget->InitializeData(PlayerNames[i], PlayerIds[i]);
		PlayerListVerticalBox->AddChild(ChildWidget);
	}

	OnPlayerChanged(PlayerNames.Num());
}



void ULobbyMenu::AddNewMessage(const FText& Message)
{
	if (Message.IsEmpty()) return;

	TObjectPtr<UChatMessage> ChildWidget = CreateWidget<UChatMessage>(GetOwningPlayer(), ChatMessageClass);
	if (!IsValid(ChildWidget)) return;

	ChildWidget->SetMessage(Message);
	MessageVerticalBox->AddChild(ChildWidget);
}
