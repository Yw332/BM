// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/BMGameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "BMGA_Combat_Y.generated.h"

/**
 * 
 */
UCLASS()
class BM_API UBMGA_Combat_Y : public UBMGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BM")
	TObjectPtr<UAnimMontage> MontageToPlay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BM")
	FName TaskInstanceName;

	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> MontageTask;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BM")
	FName SectionStartName = NAME_None;
};
