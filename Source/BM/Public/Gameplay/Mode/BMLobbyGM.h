// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/BMGameMode.h"
#include "BMLobbyGM.generated.h"

/**
 * 
 */
UCLASS()
class BM_API ABMLobbyGM : public ABMGameMode
{
	GENERATED_BODY()

public:
	virtual void StartGame();

protected:
	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void PCOnChanged();

	UPROPERTY()
	TArray<TWeakObjectPtr<APlayerController>> PCArray;

	UPROPERTY(EditDefaultsOnly, Category = "Game Mode")
	FString GameMapPath = FString("/Game/Mishima_DOJO/Levels/Showcase1");
	// 
};
