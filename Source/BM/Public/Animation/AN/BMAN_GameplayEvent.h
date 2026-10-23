// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BMAnimNotify.h"
#include "GameplayTagContainer.h"
#include "BMAN_GameplayEvent.generated.h"

/**
 * 
 */
UCLASS()
class BM_API UBMAN_GameplayEvent : public UBMAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BM")
	FGameplayTag EventTag;
};
