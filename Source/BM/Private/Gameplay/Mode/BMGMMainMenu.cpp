// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Mode/BMGMMainMenu.h"

#include "RetSessionsSubsystem.h"
#include "System/BMGlobalSubsystem.h"

void ABMGMMainMenu::BeginPlay()
{
	Super::BeginPlay();
	
	DestroyeSession();
}

void ABMGMMainMenu::DestroyeSession()
{
	if (GetGameInstance()->GetSubsystem<UBMGlobalSubsystem>()->MultiRoomType == EBMMultiRoomType::LAN)
	{
		DestroySession_BP();
	}
	else
	{
		UWorld* World = GetWorld();
		if (!World) return;
		URetSessionsSubsystem* Subsystem = World->GetGameInstance()->GetSubsystem<URetSessionsSubsystem>();
		if (Subsystem)
		{
			Subsystem->DestroySession();
		}
	}
}
