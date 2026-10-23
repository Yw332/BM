// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BMGameplayEffect.h"
#include "Chooser.h"
#include "IObjectChooser.h"
#include "ES/BMGameplayTags.h"
#include "ES/BMAbilityTypes.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "BMGameplayAbility.generated.h"

class UAbilityTask_PlayMontageAndWait;
/**
 * 
 */
UCLASS()
class BM_API UBMGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	TSubclassOf<UGameplayEffect> GetInfiniteEffectClass() const { return InfiniteEffectClass; }
	TSubclassOf<UGameplayEffect> GetInstantEffectClass() const { return InstantEffectClass; }
	float GetHitDamage() const { return HitDamage; }
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual UAnimMontage* GetMontageFromChooser(FChooserEvaluationContext& Context , UChooserTable* Chooser);

protected:
	// Used to  apply an effect when ANS begin	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BM")
	TSubclassOf<UGameplayEffect> InfiniteEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BM")
	TSubclassOf<UGameplayEffect> InstantEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BM")
	float HitDamage;
	
};
