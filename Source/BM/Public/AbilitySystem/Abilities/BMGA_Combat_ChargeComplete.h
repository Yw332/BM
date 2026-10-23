// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BMGA_Combat_Y.h"
#include "BMGA_Combat_ChargeComplete.generated.h"

/**
 * 
 */
UCLASS()
class BM_API UBMGA_Combat_ChargeComplete : public UBMGA_Combat_Y
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UAnimMontage* GetCompleteMontageToPlay();
	
	UFUNCTION()
	virtual void OnMontageCompleted();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BM")
	UChooserTable* ChargeChooser;
};
