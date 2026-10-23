// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ANS/Tag/BMANS_GameplayTag.h"

#include "AbilitySystem/GameplayEffect/BMGameplayTagEffect.h"

void UBMANS_GameplayTag::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (!MeshComp || !MeshComp->GetOwner()) return;
	
	if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(MeshComp->GetOwner()))
	{
		if (UBMAbilitySystemComponent* ASC = Cast<UBMAbilitySystemComponent>(ASI->GetAbilitySystemComponent()))
		{
			FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
			FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(InfiniteEffectClass, 1.f, Context);
			FGameplayEffectSpec* EffectSpecPtr = Spec.Data.Get();
			EffectSpecPtr->DynamicGrantedTags.AppendTags(TagsToAdd);
			EffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecPtr);

			UE_LOG(LogTemp, Warning, TEXT("Apply Effect Handle : %s"), *EffectHandle.ToString())
		}
	}
}

void UBMANS_GameplayTag::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (!MeshComp || !MeshComp->GetOwner()) return;
	
	if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(MeshComp->GetOwner()))
	{
		if (UBMAbilitySystemComponent* ASC = Cast<UBMAbilitySystemComponent>(ASI->GetAbilitySystemComponent()))
		{
			ASC->RemoveActiveGameplayEffect(EffectHandle);
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Remove Effect Handle : %s"), *EffectHandle.ToString())
}
