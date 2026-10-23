// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FindSessionsCallbackProxy.h"
#include "RetSessionsSubsystem.h"
#include "RoomResult.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UI/UIPreDefine.h"
#include "MultiRoom.generated.h"

class FOnlineSessionSearchResult;
class UEditableText;
class UButton;
/**
 * 
 */
UCLASS()
class BM_API UMultiRoomWidget : public UBMUserWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
protected:
	UFUNCTION(BlueprintCallable)
	virtual void CreateRoomButtonClicked();
	UFUNCTION(BlueprintCallable)
	virtual void SearchRoomButtonClicked();
	UFUNCTION(BlueprintCallable)
	virtual void OnCreateRoom(bool bWasSuccessful);
	virtual void OnSearchRooms(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);

	UFUNCTION(BlueprintImplementableEvent)
	void CreateLANRoom_BP();
	
	UFUNCTION(BlueprintImplementableEvent)
	void SearchLANRoom_BP();
	
protected:
	
	UPROPERTY(BlueprintReadWrite, Category="BM");
	int32 PlayerNum = 2;
	
	UPROPERTY(BlueprintReadWrite, Category="BM")
	EBMMultiRoomType MultiRoomType = EBMMultiRoomType::LAN;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> CreateRoomButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> SearchRoomsButton;

	UPROPERTY(meta=(BindWidget), BlueprintReadWrite)
	TObjectPtr<UScrollBox> RoomListScrollBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<URoomResult> RoomResultClass;

	UPROPERTY()
	TObjectPtr<URetSessionsSubsystem> RetSessionsSubsystem;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BM");
	FString Password = FString("BMNone");

	
};


