// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Chat/ChatSender.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Player/Controller/BMPCLobby.h"
#include "Player/PlayerState/BMPSLobby.h"

void UChatSender::NativeConstruct()
{
	Super::NativeConstruct();

	SendButton->OnClicked.AddDynamic(this, &UChatSender::SendMessage);
}

void UChatSender::SendMessage()
{
	FText Message = MessageText->GetText();
	if (Message.IsEmpty()) return;
	
	ABMPCLobby* PC = Cast<ABMPCLobby>(GetOwningPlayer());
	if (PC == nullptr) return;
	ABMPSLobby* PS = PC->GetPlayerState<ABMPSLobby>();
	if (PS == nullptr) return;
	
	Message = FText::Format(FText::FromString("{0}: {1}"), FText::FromString(PS->GetPlayerName().ToString()), Message);

	PC->Server_SendMessage(Message);
	MessageText->SetText(FText::GetEmpty());
}
