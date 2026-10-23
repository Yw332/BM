// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Base/BMUserWidgetBase.h"

#include "UMGPCInfo.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class BM_API UUMGPCInfo : public UBMUserWidgetBase
{
	GENERATED_BODY()

public:

	void InitializeData(FName PlayerName, uint32 PlayerId);

	UFUNCTION(BlueprintImplementableEvent)
	void DestroySession_BP();

	UFUNCTION(BlueprintCallable)
	void SetLogOutButtonEnabled(bool bEnable);
	
	UFUNCTION(BlueprintCallable)
	void LogOut();

	UFUNCTION(BlueprintCallable)
	void SetPlayerName(FName PlayerName);
	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> LogOutButton;
};
