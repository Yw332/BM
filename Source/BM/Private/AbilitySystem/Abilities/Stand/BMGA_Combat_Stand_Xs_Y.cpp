// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Stand/BMGA_Combat_Stand_Xs_Y.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

void UBMGA_Combat_Stand_Xs_Y::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                        const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	check(MontageToPlay);
	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, MontageToPlay, 1.0f, FName("Start"));
	if (!MontageTask)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
        return;
	}

    MontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnMontageCompleted);
	MontageTask->ReadyForActivation();

	UWorld* World = GetAvatarActorFromActorInfo()->GetWorld();
	FTimerDelegate ChargeTimerDelegate = FTimerDelegate::CreateLambda([this]()
	{
        MontageSetNextSectionName(FName("Start"), FName("Loop"));
        MontageSetNextSectionName(FName("Loop"), FName("Loop"));
	});
	World->GetTimerManager().SetTimerForNextTick(ChargeTimerDelegate);

	FGameplayTag EventTag = FBMGameplayTags::Get().GameplayEvent_Animation_ChargeStartComplete;
	WaitStartMontageTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this,EventTag,nullptr, true, true);
	if (IsValid(WaitStartMontageTask))
	{
		WaitStartMontageTask->EventReceived.AddDynamic(this, &ThisClass::OnChargeStartMontageCompleted);
		WaitStartMontageTask->ReadyForActivation();
	}

}

void UBMGA_Combat_Stand_Xs_Y::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UBMGA_Combat_Stand_Xs_Y::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UBMGA_Combat_Stand_Xs_Y::OnAbilityCompleted()
{
	MontageJumpToSection(FName("Complete"));
}

void UBMGA_Combat_Stand_Xs_Y::OnChargeStartMontageCompleted(FGameplayEventData Payload)
{
	
	WaitCompleteTagTask = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(this, FBMGameplayTags::Get().Input_Charge_Complete, nullptr,true);
	if (!IsValid(WaitCompleteTagTask)) return;
	WaitCompleteTagTask->Added.AddDynamic(this, &ThisClass::OnAbilityCompleted);
	WaitCompleteTagTask->ReadyForActivation();

	MontageJumpToSection(FName("Loop"));
}
