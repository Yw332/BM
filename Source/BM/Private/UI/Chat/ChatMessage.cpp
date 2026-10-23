// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Chat/ChatMessage.h"
#include "Components/TextBlock.h"

void UChatMessage::SetMessage(const FText& Message)
{
	MessageText->SetText(Message);
}
