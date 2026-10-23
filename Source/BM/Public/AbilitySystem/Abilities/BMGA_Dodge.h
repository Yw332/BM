// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chooser.h"
#include "AbilitySystem/BMGameplayAbility.h"
#include "BMGA_Dodge.generated.h"

class UAbilityTask_WaitGameplayTagAdded;
class UAbilityAsync_WaitGameplayTagAdded;
/**
 * 
 */
UCLASS()
class BM_API UBMGA_Dodge : public UBMGameplayAbility
{
	GENERATED_BODY()
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION()
	void OnHitTagAdded();
	
	UFUNCTION()
	virtual void OnMontageCompleted();

	void OnPerfectDodgeTriggered();
	float GetCurrentMontageTime();

	UAnimMontage* GetDodgeMontageToPlay();
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BM")
	UChooserTable* DodgeChooser;

	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayTagAdded> WaitTagTask;

	UPROPERTY(BlueprintReadOnly, Category = "BM")
	bool bPerfectDodgeTriggered = false;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BM")
	TObjectPtr<UAnimMontage> MontageToPlay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BM")
	FName TaskInstanceName;

	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> MontageTask;
};
