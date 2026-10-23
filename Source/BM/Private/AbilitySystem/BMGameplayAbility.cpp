// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/BMGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "ChooserFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "Animation/BMAnimLayerInstance.h"
#include "Player/Character/BMPlayerCharacter.h"

void UBMGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to commit ability in %s"), *GetName());
		EndAbility(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo,true,true);
		return;
	}
}

void UBMGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

UAnimMontage* UBMGameplayAbility::GetMontageFromChooser(FChooserEvaluationContext& Context , UChooserTable* Chooser)
{
	if (!Chooser)  return nullptr;
	// 存储结果的变量
	UAnimMontage* SelectedMontage = nullptr;

	// 创建回调函数
	FObjectChooserBase::FObjectChooserIteratorCallback Callback;
	Callback.BindLambda([&SelectedMontage](UObject* Object) -> FObjectChooserBase::EIteratorStatus
	{
		if (UAnimMontage* Montage = Cast<UAnimMontage>(Object))
		{
			SelectedMontage = Montage;
			return FObjectChooserBase::EIteratorStatus::Stop; // 找到第一个有效结果就停止
		}
		return FObjectChooserBase::EIteratorStatus::Continue; // 继续查找
	});

	// 执行评估
	FObjectChooserBase::EIteratorStatus Status = UChooserTable::EvaluateChooser(Context, Chooser ,Callback);

	UE_LOG(LogTemp, Warning, TEXT("Status: %d"), (int)Status);
	UE_LOG(LogTemp, Warning, TEXT("SelectedMontage: %s"), *SelectedMontage->GetName());
	
	if (Status == FObjectChooserBase::EIteratorStatus::Stop && SelectedMontage)
	{
		return SelectedMontage;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Chooser failed to find valid AnimMontage in %s"), *GetName());
		return nullptr;
	}
}
