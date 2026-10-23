// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AN/BMAN_GameplayEvent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UBMAN_GameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                 const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (!EventTag.IsValid()) return;
	if (!MeshComp->GetOwner()) return;
	if (!MeshComp->GetWorld()) return;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), EventTag, FGameplayEventData());
}
