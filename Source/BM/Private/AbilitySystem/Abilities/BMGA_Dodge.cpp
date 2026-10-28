// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/BMGA_Dodge.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "Animation/BMAnimLayerInstance.h"
#include "Player/Character/BMPlayerCharacter.h"
#include "Player/Character/BMBaseCharacter.h"

void UBMGA_Dodge::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	FGameplayTag HitTag = FBMGameplayTags::Get().GameplayEvent_Hit;
	WaitTagTask = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(this, HitTag, /*OptionalExternalTarget=*/nullptr, /*OnlyOnce=*/true);
	if (!WaitTagTask)
	{
		EndAbility(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo,true,false);
		return;
	}
	WaitTagTask->Added.AddDynamic(this, &UBMGA_Dodge::OnHitTagAdded);
	WaitTagTask->ReadyForActivation();
	
	MontageToPlay = GetDodgeMontageToPlay();
	if (!IsValid(MontageToPlay)) 
	{
		EndAbility(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo,true,false);
		UE_LOG(LogTemp, Error,TEXT("Dodge Chooser returned invalid result in %s"), *GetName());
		return;
	}
	
	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TaskInstanceName, MontageToPlay);

	if (!IsValid(MontageTask)) 
	{
		EndAbility(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo,true,false);
		return;
	}
	
	MontageTask->OnCompleted.AddDynamic(this,&ThisClass::OnMontageCompleted);
	MontageTask->ReadyForActivation();

	// 闪避无敌帧：原来没有任何无敌逻辑，所以闪避与直接吃伤害没区别。
	// 这里按蒙太奇总时长乘一个比例开一个无敌窗，窗口内 DoDamageToActor 会直接丢弃伤害。
	BeginInvincibleWindow();
}

void UBMGA_Dodge::BeginInvincibleWindow()
{
	ABMBaseCharacter* OwnCharacter = Cast<ABMBaseCharacter>(GetAvatarActorFromActorInfo());
	if (!IsValid(OwnCharacter)) return;

	OwnCharacter->SetInvincible(true);

	// 算无敌窗长度：蒙太奇时长 × 比例，兜底用 0.4 秒。
	float WindowLength = 0.4f;
	if (IsValid(MontageToPlay))
	{
		const float Len = MontageToPlay->GetPlayLength();
		if (Len > 0.f)
		{
			WindowLength = Len * FMath::Clamp(InvincibleEndRatio, 0.05f, 1.f);
		}
	}

	OwnCharacter->GetWorldTimerManager().SetTimer(
		InvincibleEndTimerHandle,
		FTimerDelegate::CreateWeakLambda(this, [this]()
		{
			EndInvincibleWindow();
		}),
		WindowLength, false);
}

void UBMGA_Dodge::EndInvincibleWindow()
{
	if (ABMBaseCharacter* OwnCharacter = Cast<ABMBaseCharacter>(GetAvatarActorFromActorInfo()))
	{
		OwnCharacter->SetInvincible(false);
	}
}

void UBMGA_Dodge::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// 闪避结束（完成、被打断、或取消）时一定要关掉无敌，
	// 否则一次中断就会让角色永久无敌。
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(InvincibleEndTimerHandle);
	}
	EndInvincibleWindow();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (WaitTagTask)
	{
		WaitTagTask->EndTask();
		WaitTagTask = nullptr;
	}

}

void UBMGA_Dodge::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	bPerfectDodgeTriggered = false;
}


void UBMGA_Dodge::OnHitTagAdded()
{
	if (!IsValid(MontageTask)) return;

	FGameplayTag PerfectDodgeTag = FBMGameplayTags::Get().Ability_Dodge_Perfect;
	if (!GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(PerfectDodgeTag)) return;

	OnPerfectDodgeTriggered();
}

void UBMGA_Dodge::OnPerfectDodgeTriggered()
{
	bPerfectDodgeTriggered = true;
	float CurrentTime = GetCurrentMontageTime();
	
	MontageToPlay = GetDodgeMontageToPlay();
	if (!IsValid(MontageToPlay))
	{
		UE_LOG(LogTemp, Error,TEXT("Perfect Dodge Chooser returned invalid result in %s"), *GetName());
		return;
	};

	MontageTask->EndTask();
	
	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TaskInstanceName, MontageToPlay,
		true,NAME_None,true, 1, 0, false);

	if (MontageTask)
	{
		MontageTask->OnCompleted.AddDynamic(this,&ThisClass::OnMontageCompleted);
		MontageTask->ReadyForActivation();
	}
}

float UBMGA_Dodge::GetCurrentMontageTime()
{
	ABMPlayerCharacter* OwnCharacter = Cast<ABMPlayerCharacter>(GetAvatarActorFromActorInfo());
	if (!IsValid(OwnCharacter)) return 0.0f;
	
	UAnimInstance* AnimInstance = OwnCharacter->GetMesh()->GetAnimInstance();
	if (!IsValid(AnimInstance)) return 0.0f;

	if (UAnimMontage* CurMontage = AnimInstance->GetCurrentActiveMontage())
	{
		return AnimInstance->Montage_GetPosition(CurMontage);
	}
	return 0.0f;
}

UAnimMontage* UBMGA_Dodge::GetDodgeMontageToPlay()
{
	ABMPlayerCharacter* OwnCharacter = Cast<ABMPlayerCharacter>(GetAvatarActorFromActorInfo());
	if (!IsValid(OwnCharacter)) return nullptr;
	UAnimInstance* AnimLayerInstance = OwnCharacter->GetMesh()->GetLinkedAnimLayerInstanceByClass(OwnCharacter->GetLinkAnimInstance());
	if (!IsValid(AnimLayerInstance)) return nullptr;
	if (!IsValid(DodgeChooser)) return nullptr;

	FChooserEvaluationContext Context;
	Context.AddObjectParam(AnimLayerInstance);
	Context.AddObjectParam(this);

	return GetMontageFromChooser(Context, DodgeChooser);
}