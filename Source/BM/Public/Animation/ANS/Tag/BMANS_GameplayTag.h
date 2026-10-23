// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BMAnimNotifyState.h"
#include "BMANS_GameplayTag.generated.h"

/**
 * 
 */
UCLASS()
class BM_API UBMANS_GameplayTag : public UBMAnimNotifyState
{
	GENERATED_BODY()
protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BM")
	TSubclassOf<UGameplayEffect> InfiniteEffectClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BM")
	FGameplayTagContainer TagsToAdd;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BM")
	FGameplayTagContainer TagsToRemove;

	UPROPERTY()
	FActiveGameplayEffectHandle EffectHandle;
};
