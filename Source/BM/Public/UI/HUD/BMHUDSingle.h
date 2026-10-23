// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BMHUD.h"
#include "BMHUDSingle.generated.h"

/**
 * 
 */
UCLASS()
class BM_API ABMHUDSingle : public ABMHUD
{
	GENERATED_BODY()

public:
	virtual void InitialOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS) override;

	
};
