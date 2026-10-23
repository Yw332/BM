// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/BMBaseCharacter.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerState.h"

ABMBaseCharacter::ABMBaseCharacter(const FObjectInitializer& ObjectInitializer)  : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

UAbilitySystemComponent* ABMBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABMBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABMBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABMBaseCharacter::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> EffectClass, float Level) const
{
}

void ABMBaseCharacter::InitDefaultAttributes()
{
}

void ABMBaseCharacter::InitAbilityActorInfo()
{
	
}

