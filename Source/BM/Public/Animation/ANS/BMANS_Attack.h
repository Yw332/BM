// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BMAnimNotifyState.h"
#include "BMANS_Attack.generated.h"

/**
 * 
 */
UCLASS()
class BM_API UBMANS_Attack : public UBMAnimNotifyState
{
	GENERATED_BODY()
	
protected:

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:

	TArray<FVector> TraceLocations;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BM")
	TArray<FName> TraceSocketNames;

	TArray<TWeakObjectPtr<AActor>> TouchedActors;
	
	FCollisionQueryParams Params;

	float HitDamage = 10; // default
	TSubclassOf<UGameplayEffect> InfiniteEffectClass;
	TSubclassOf<UGameplayEffect> InstantEffectClass;
};
