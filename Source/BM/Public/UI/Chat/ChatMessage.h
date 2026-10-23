// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Base/BMUserWidgetBase.h"
#include "ChatMessage.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class BM_API UChatMessage : public UBMUserWidgetBase
{
	GENERATED_BODY()

public:
	void SetMessage(const FText& Message);


protected:
	
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UTextBlock* MessageText;

	

	
};
