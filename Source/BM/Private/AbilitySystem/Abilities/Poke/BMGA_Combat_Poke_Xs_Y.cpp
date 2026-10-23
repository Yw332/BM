// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Poke/BMGA_Combat_Poke_Xs_Y.h"

void UBMGA_Combat_Poke_Xs_Y::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	check(MontageToPlay);
	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TaskInstanceName,MontageToPlay);

	if (!MontageTask)
	{
		EndAbility(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo,true,false);
		return;
	}
	
	MontageTask->OnCompleted.AddDynamic(this,&ThisClass::OnMontageCompleted);
	MontageTask->ReadyForActivation();
}

void UBMGA_Combat_Poke_Xs_Y::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UBMGA_Combat_Poke_Xs_Y::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo,true,false);
}
