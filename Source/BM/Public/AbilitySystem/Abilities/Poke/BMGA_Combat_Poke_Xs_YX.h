// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BMGA_Combat_Y.h"
#include "BMGA_Combat_Poke_Xs_YX.generated.h"

class UAbilityTask_WaitGameplayEvent;
/**
 * 
 */
UCLASS()
class BM_API UBMGA_Combat_Poke_Xs_YX : public UBMGA_Combat_Y
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnAbilityCompleted();

	UFUNCTION()
	void OnChargeStartMontageCompleted(FGameplayEventData Payload);
	
protected:
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitStartMontageTask;

	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayTagAdded> WaitCompleteTagTask;
};
