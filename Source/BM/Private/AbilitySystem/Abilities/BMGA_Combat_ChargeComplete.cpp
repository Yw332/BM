// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/BMGA_Combat_ChargeComplete.h"
#include "AbilitySystem/BMAbilitySystemComponent.h"
#include "AbilitySystem/BMAttributeSet.h"
#include "AbilitySystem/Abilities/BMGA_Combat_Charge.h"

void UBMGA_Combat_ChargeComplete::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                  const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	MontageToPlay = GetCurrentMontage();
	if (!IsValid(MontageToPlay)) return;

	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TaskInstanceName, MontageToPlay);
	if (!IsValid(MontageTask))
	{
		EndAbility(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo,true,true);
		return;
	};
	MontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnMontageCompleted);
	MontageTask->ReadyForActivation();
}

void UBMGA_Combat_ChargeComplete::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	if (IsValid(MontageTask))
	{
		MontageTask->EndTask();
		MontageTask->OnCompleted.Clear();
		MontageTask = nullptr;
	}
}

UAnimMontage* UBMGA_Combat_ChargeComplete::GetCompleteMontageToPlay()
{
	UBMAbilitySystemComponent* AbilitySystemComponent = Cast<UBMAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	if (!IsValid(AbilitySystemComponent)) return nullptr;
	const UBMAttributeSet* AttributeSet = Cast<UBMAttributeSet>(AbilitySystemComponent->GetAttributeSet(UBMAttributeSet::StaticClass()));
	if (!IsValid(AttributeSet)) return nullptr;

	FBMChargeMontageData MontageData;
	MontageData.StickMomentumLevel = AttributeSet->GetStickMomentumLevel();
	MontageData.ChargeLevel = EBMChargeLevel::Complete;
	MontageData.CombatType = AbilitySystemComponent->GetCombatType();

	FChooserEvaluationContext Context;
	Context.AddStructParam(MontageData);

	return GetMontageFromChooser(Context, ChargeChooser);
}

void UBMGA_Combat_ChargeComplete::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
