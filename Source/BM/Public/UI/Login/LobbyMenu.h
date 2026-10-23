// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Base/BMUserWidgetBase.h"
#include "LobbyMenu.generated.h"

class UChatMessage;
class UUMGPCInfo;
class UVerticalBox;
class ABMLobbyGM;
class UButton;
/**
 * 
 */
UCLASS()
class BM_API ULobbyMenu : public UBMUserWidgetBase
{
	GENERATED_BODY()

public:

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnPlayerChanged(int32 PlayerCount);
	UFUNCTION()
	void StartGame();

	UFUNCTION()
	void UpdatePlayerList(const TArray<FName>& PlayerNames, const TArray<uint32>& PlayerIds);

	UFUNCTION(BlueprintCallable)
	void AddNewMessage(const FText& Message);
	
protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	TObjectPtr<UButton> StartGameButton;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	TObjectPtr<UVerticalBox> PlayerListVerticalBox ;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	TObjectPtr<UVerticalBox> MessageVerticalBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UChatMessage> ChatMessageClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUMGPCInfo> PlayerListClass;
	
	UPROPERTY()
	TObjectPtr<ABMLobbyGM> GameMode = nullptr;

};
