// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BMUserWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class BM_API UBMUserWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = "BM|UI")
	virtual bool IsServer();


	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController = nullptr;
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSetup();
	
};
