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

	/** 命中时把目标往后推的水平速度（cm/s）。0 = 不击退。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BM|Knockback")
	float KnockbackStrength = 700.f;

	/** 击退时附加的向上速度（cm/s）。0 = 纯水平后退，不用离地。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BM|Knockback")
	float KnockbackUpward = 80.f;

	/** 击退方向是否跟目标当前速度相反（true）还是沿攻击方向（false）。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BM|Knockback")
	bool bKnockbackAwayFromAttacker = false;
};
