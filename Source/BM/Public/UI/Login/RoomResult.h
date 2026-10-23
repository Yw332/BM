// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FindSessionsCallbackProxy.h"
#include "RetSessionsSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"
#include "Components/Button.h"
#include "UI/Base/BMUserWidgetBase.h"
#include "RoomResult.generated.h"

class FOnlineSessionSearchResult;
class UTextBlock;
class UButton;

/**
 * 
 */
UCLASS()
class BM_API URoomResult : public UBMUserWidgetBase
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	UFUNCTION(BlueprintCallable)
	virtual void InitRoomResult(float InPingInMs, int32 InCurrentPlayers, int32 InMaxPlayers, const FString& InRoomName);
	virtual void SetSearchResult(const FOnlineSessionSearchResult& InSearchResult) { SearchResult = InSearchResult; }
	UFUNCTION(BlueprintCallable)
	virtual void SetSearchResult(const FBlueprintSessionResult& InSearchResult) { SearchResult_BP = InSearchResult; }
	
protected:

	UFUNCTION()
	virtual void JoinRoomButtonClicked();

	UFUNCTION(BlueprintImplementableEvent)
	void JoinRoom_BP();

	void OnJoinRoom(EOnJoinSessionCompleteResult::Type Result);
	
	UPROPERTY()
	TObjectPtr<URetSessionsSubsystem> RetSessionsSubsystem;
	
	UPROPERTY(BlueprintReadWrite, Category="Room")
	float PingInMs = 0.0f;

	UPROPERTY(BlueprintReadWrite, Category="Room")
	int32 CurrentPlayers = 0;
	
	UPROPERTY(BlueprintReadWrite, Category="Room")
	int32 MaxPlayers = 0;

	UPROPERTY(BlueprintReadWrite, Category="Room")
	FString RoomName = FString("Unknown");
	
	FOnlineSessionSearchResult SearchResult;

	UPROPERTY(BlueprintReadOnly, Category="Room")
	FBlueprintSessionResult SearchResult_BP;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> PingTextBlock;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> PlayersTextBlock;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> RoomNameTextBlock;

	/** Logic in Blueprint */
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> JoinRoomButton;
};
