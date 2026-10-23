// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/BMGameMode.h"
#include "BMGMSingle.generated.h"

/**
 * 
 */
UCLASS()
class BM_API ABMGMSingle : public ABMGameMode
{
	GENERATED_BODY()

public:
	virtual void StartPlay() override;
	
};
