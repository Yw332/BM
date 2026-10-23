// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BMGA_Combat_Y.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "BMGA_Combat_Charge.generated.h"

class UChooserTable;


/**
 *  立棍逻辑包含其他棍法。
 */
UCLASS()
class BM_API UBMGA_Combat_Charge : public UBMGA_Combat_Y
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UFUNCTION()
	void OnChargeStartMontageCompleted(FGameplayEventData Payload);

	UFUNCTION()
	void OnChargeComplete();

	UFUNCTION()
	void OnChargeLevelUp();
	
	UFUNCTION()
	void OnChargeLoop();

	UFUNCTION()
	void OnMontageCompleted();
	UFUNCTION()
	void OnAbilityCompleted(FGameplayEventData Payload);
	
	UAnimMontage* GetChargeMontageToPlay();
	FName GetChargeSectionToPlay();

	FORCEINLINE FName SectionStart(int32 Level) const { return bHasLevelUpAnimation ? FName(FString::Printf(TEXT("Start_L%d"), Level)) : FName(TEXT("Start")); }
    FORCEINLINE FName SectionLoop(int32 Level) const { return bHasLevelUpAnimation ? FName(FString::Printf(TEXT("Loop_L%d"), Level)) : FName(TEXT("Loop")); }
	static FORCEINLINE FName SectionComplete(int32 Level) { return FName(FString::Printf(TEXT("Complete_L%d"), Level)); }
	static FORCEINLINE FName SectionLevelUp(int32 Level) { return FName(FString::Printf(TEXT("LevelUp_L%d"), Level)); }

protected:
	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BM")
	UChooserTable* ChargeChooser;*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BM")
	bool bHasLevelUpAnimation = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BM")
	bool bRootHasZAxisMovement = false;
	
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayTagAdded> WaitCompleteTagTask;

	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitStartMontageTask;

	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitCompleteMontageTask;
	
	UPROPERTY()
	EBMChargeLevel CurrentChargeLevel = EBMChargeLevel::Start;

	bool bIsAuthority = false;

	int32 ChargeLevel = 0;
};
