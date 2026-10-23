// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/BMUIMultiOverlayController.h"

#include "AbilitySystem/BMAbilitySystemComponent.h"

void UBMUIMultiOverlayController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();

	const UBMAttributeSet* AS = Cast<UBMAttributeSet>(AttributeSet);
	OnHealthChanged.Broadcast(AS->GetHealth());
	OnStaminaChanged.Broadcast(AS->GetStamina());
	OnStickMomentumChanged.Broadcast(AS->GetStickMomentum());
	OnStickMomentumLevelChanged.Broadcast(AS->GetStickMomentumLevel());

	const UBMAbilitySystemComponent* ASC = Cast<UBMAbilitySystemComponent>(AbilitySystemComponent);
	OnStickTypeChanged.Broadcast((int32)ASC->GetCombatType());
}

void UBMUIMultiOverlayController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

	const UBMAttributeSet* AS = Cast<UBMAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);	
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetStaminaAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
		{
			OnStaminaChanged.Broadcast(Data.NewValue);	
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetStickMomentumAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
		{
			OnStickMomentumChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetStickMomentumLevelAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnStickMomentumLevelChanged.Broadcast(Data.NewValue);
		}
	);

	UBMAbilitySystemComponent* ASC = Cast<UBMAbilitySystemComponent>(AbilitySystemComponent);
	ASC->OnStickTypeChanged.AddLambda(
		[this](int32 NewValue)
		{
			OnStickTypeChanged.Broadcast(NewValue);
		}
	);
}
