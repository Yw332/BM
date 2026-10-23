// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/BMGA_Combat_Charge.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/BMAbilitySystemComponent.h"
#include "AbilitySystem/BMAttributeSet.h"

void UBMGA_Combat_Charge::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                          const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (bRootHasZAxisMovement)
	{
		ACharacter* TempCharacter = Cast<ACharacter>(GetAvatarActorFromActorInfo());
        if (IsValid(TempCharacter))
        {
            TempCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
        }
	}
	
	check(MontageToPlay);
	
	CurrentChargeLevel = EBMChargeLevel::Start;
	const UBMAttributeSet* AttributeSet = Cast<UBMAttributeSet>(GetAbilitySystemComponentFromActorInfo()->GetAttributeSet(UBMAttributeSet::StaticClass()));
	if (!IsValid(AttributeSet)) return;
	
	ChargeLevel = AttributeSet->GetStickMomentumLevel();
	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, MontageToPlay, 1, SectionStart(ChargeLevel));
	if (!IsValid(MontageTask))
	{
		EndAbility(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo,true,true);
		return;
	};
	
	MontageSetNextSectionName(SectionStart(ChargeLevel), SectionLoop(ChargeLevel));
	MontageSetNextSectionName(SectionLoop(ChargeLevel), SectionLoop(ChargeLevel));
	
	MontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnMontageCompleted);
	MontageTask->ReadyForActivation();

	FGameplayTag EventTag = FBMGameplayTags::Get().GameplayEvent_Animation_ChargeStartComplete;
	WaitStartMontageTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this,EventTag,nullptr, true, true);
	if (IsValid(WaitStartMontageTask))
	{
		WaitStartMontageTask->EventReceived.AddDynamic(this, &ThisClass::OnChargeStartMontageCompleted);
		WaitStartMontageTask->ReadyForActivation();
	}

	UWorld* World = GetAvatarActorFromActorInfo()->GetWorld();
	FTimerDelegate ChargeTimerDelegate = FTimerDelegate::CreateLambda([this]()
	{
		MontageSetNextSectionName(SectionStart(ChargeLevel), SectionLoop(ChargeLevel));
		MontageSetNextSectionName(SectionLoop(ChargeLevel), SectionLoop(ChargeLevel));
	});
	World->GetTimerManager().SetTimerForNextTick(ChargeTimerDelegate);
	
	if (ActorInfo->IsNetAuthority())
	{
		bIsAuthority = true;
		UBMAbilitySystemComponent* ASC = Cast<UBMAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
		if (!IsValid(ASC)) return;
		ASC->ChargeStickMomentumStart();
	}
}

void UBMGA_Combat_Charge::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (IsValid(MontageTask))
	{
		MontageTask->EndTask();
		MontageTask->OnCompleted.Clear();
		MontageTask = nullptr;
	}

	if (IsValid(WaitStartMontageTask))
	{
		WaitStartMontageTask->EndTask();
		WaitStartMontageTask->EventReceived.Clear();
		WaitStartMontageTask = nullptr;
	}
	
	if (IsValid(WaitCompleteTagTask))
	{
		WaitCompleteTagTask->EndTask();
		WaitCompleteTagTask->Added.Clear();
		WaitCompleteTagTask = nullptr;
	}

	if (bRootHasZAxisMovement)
	{
		ACharacter* TempCharacter = Cast<ACharacter>(GetAvatarActorFromActorInfo());
		if (IsValid(TempCharacter))
		{
			TempCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		}
	}
}

void UBMGA_Combat_Charge::OnChargeStartMontageCompleted(FGameplayEventData Payload)
{
	CurrentChargeLevel = EBMChargeLevel::Loop;
	UBMAbilitySystemComponent* ASC = Cast<UBMAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	if (ASC)
	{
		ASC->OnStickMomentumLevelUpDelegate.AddDynamic(this, &ThisClass::OnChargeLevelUp);
	}

	WaitCompleteTagTask = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(this, FBMGameplayTags::Get().Input_Charge_Complete);
	if (!IsValid(WaitCompleteTagTask)) return;
	WaitCompleteTagTask->Added.AddDynamic(this, &ThisClass::OnChargeComplete);
	WaitCompleteTagTask->ReadyForActivation();

	const UBMAttributeSet* AttributeSet = Cast<UBMAttributeSet>(GetAbilitySystemComponentFromActorInfo()->GetAttributeSet(UBMAttributeSet::StaticClass()));
	if (!IsValid(AttributeSet)) return;
	ChargeLevel = AttributeSet->GetStickMomentumLevel();
	
	MontageJumpToSection(SectionLoop(ChargeLevel));
	MontageSetNextSectionName(SectionLoop(ChargeLevel), SectionLoop(ChargeLevel));
}

void UBMGA_Combat_Charge::OnChargeComplete()
{
	CurrentChargeLevel = EBMChargeLevel::Complete;
	UBMAbilitySystemComponent* ASC = Cast<UBMAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	if (IsValid(ASC))
	{
		if (bIsAuthority) ASC->ChargeStickMomentumComplete();
		ASC->OnStickMomentumLevelUpDelegate.RemoveDynamic(this, &ThisClass::OnChargeLevelUp);
	}

	const UBMAttributeSet* AttributeSet = Cast<UBMAttributeSet>(GetAbilitySystemComponentFromActorInfo()->GetAttributeSet(UBMAttributeSet::StaticClass()));
	if (IsValid(AttributeSet))
	{
		ChargeLevel = AttributeSet->GetStickMomentumLevel();
	}
	
	MontageJumpToSection(SectionComplete(ChargeLevel));
	MontageSetNextSectionName(SectionComplete(ChargeLevel), NAME_None);
}

void UBMGA_Combat_Charge::OnChargeLevelUp()
{
	if (CurrentChargeLevel == EBMChargeLevel::Complete)  return;
	if (!bHasLevelUpAnimation) return;
	CurrentChargeLevel = EBMChargeLevel::LevelUp;
	
	const UBMAttributeSet* AttributeSet = Cast<UBMAttributeSet>(GetAbilitySystemComponentFromActorInfo()->GetAttributeSet(UBMAttributeSet::StaticClass()));
	if (IsValid(AttributeSet))
	{
		ChargeLevel = AttributeSet->GetStickMomentumLevel();
	}

	MontageJumpToSection(SectionLevelUp(ChargeLevel));
	MontageSetNextSectionName(SectionLevelUp(ChargeLevel), SectionLoop(ChargeLevel));
	MontageSetNextSectionName(SectionLoop(ChargeLevel), SectionLoop(ChargeLevel));
}

void UBMGA_Combat_Charge::OnChargeLoop()
{
	/*if (CurrentChargeLevel == EBMChargeLevel::Complete)  return;
	CurrentChargeLevel = EBMChargeLevel::Loop;
	
	MontageTask->OnCompleted.Clear();
	MontageTask->EndTask();
	MontageToPlay = GetChargeMontageToPlay();
	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TaskInstanceName, MontageToPlay);
	if (!IsValid(MontageTask)) return;
	
	MontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnChargeLoop);
	MontageTask->ReadyForActivation();

	
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!IsValid(ASC)) return;
	FName SectionName = GetChargeSectionToPlay();
	ASC->CurrentMontageJumpToSection(SectionName);
	MontageSetNextSectionName(MontageToPlay, SectionName, SectionName);*/
}

void UBMGA_Combat_Charge::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo,true,false);
}

void UBMGA_Combat_Charge::OnAbilityCompleted(FGameplayEventData Payload)
{
	EndAbility(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo,true,false);
}

UAnimMontage* UBMGA_Combat_Charge::GetChargeMontageToPlay()
{
	/*UBMAbilitySystemComponent* AbilitySystemComponent = Cast<UBMAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	if (!IsValid(AbilitySystemComponent)) return nullptr;
	const UBMAttributeSet* AttributeSet = Cast<UBMAttributeSet>(AbilitySystemComponent->GetAttributeSet(UBMAttributeSet::StaticClass()));
	if (!IsValid(AttributeSet)) return nullptr;

	FBMChargeMontageData MontageData;
	MontageData.StickMomentumLevel = AttributeSet->GetStickMomentumLevel();
	MontageData.ChargeLevel = CurrentChargeLevel;
	MontageData.CombatType = AbilitySystemComponent->GetCombatType();
	
	FChooserEvaluationContext Context;
	Context.AddStructParam(MontageData);

	return GetMontageFromChooser(Context, ChargeChooser);*/
	return nullptr;
}

FName UBMGA_Combat_Charge::GetChargeSectionToPlay()
{
	/*const UBMAttributeSet* AttributeSet = Cast<UBMAttributeSet>(GetAbilitySystemComponentFromActorInfo()->GetAttributeSet(UBMAttributeSet::StaticClass()));
	if (!IsValid(AttributeSet)) return NAME_None;
	int32 StickMomentumLevel = AttributeSet->GetStickMomentumLevel();
	
	switch (CurrentChargeLevel)
	{
		case EBMChargeLevel::Start:
			return SectionStart(StickMomentumLevel);
		case EBMChargeLevel::LevelUp:
			return SectionLevelUp(StickMomentumLevel);
		case EBMChargeLevel::Loop:
			return SectionLoop(StickMomentumLevel);
		case EBMChargeLevel::Complete:
			return SectionComplete(StickMomentumLevel);
		default:
			return NAME_None;
	}*/
	return NAME_None;
}

