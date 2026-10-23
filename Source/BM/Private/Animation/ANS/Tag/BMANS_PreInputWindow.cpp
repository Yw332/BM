// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ANS/Tag/BMANS_PreInputWindow.h"

void UBMANS_PreInputWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if (!MeshComp || !MeshComp->GetOwner()) return;

	if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(MeshComp->GetOwner()))
	{
		if (UBMAbilitySystemComponent* ASC = Cast<UBMAbilitySystemComponent>(ASI->GetAbilitySystemComponent()))
		{
			ASC->SetPreInputTag(FGameplayTag());
		}
	}
}

void UBMANS_PreInputWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	if (!MeshComp || !MeshComp->GetOwner()) return;

	if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(MeshComp->GetOwner()))
	{
		if (UBMAbilitySystemComponent* ASC = Cast<UBMAbilitySystemComponent>(ASI->GetAbilitySystemComponent()))
		{
			ASC->RemoveActiveGameplayEffect(EffectHandle);

			FGameplayTagContainer AbilityTags = FGameplayTagContainer(ASC->GetPreInputTag());
			ASC->TryActivateAbilitiesByTag(AbilityTags);
			ASC->SetPreInputTag(FGameplayTag());
		}
	}
}
