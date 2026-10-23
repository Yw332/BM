// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Base/BMUserWidgetBase.h"
#include "ChatSender.generated.h"

class UEditableText;
class UButton;
/**
 * 
 */
UCLASS()
class BM_API UChatSender : public UBMUserWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	virtual void SendMessage();


protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* SendButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UEditableText* MessageText;
};
