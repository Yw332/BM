// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/BMAbilitySystemComponent.h"
#include "AbilitySystem/BMGameplayEffect.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "BMAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class BM_API UBMAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
