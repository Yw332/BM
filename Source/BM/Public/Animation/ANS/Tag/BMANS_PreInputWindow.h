// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BMANS_GameplayTag.h"
#include "../BMAnimNotifyState.h"
#include "BMANS_PreInputWindow.generated.h"

/**
 * 
 */
UCLASS()
class BM_API UBMANS_PreInputWindow : public UBMANS_GameplayTag
{
	GENERATED_BODY()

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
