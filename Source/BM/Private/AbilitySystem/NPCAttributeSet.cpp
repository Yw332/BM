// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/NPCAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/BMAbilitySystemComponent.h"

UNPCAttributeSet::UNPCAttributeSet()
{
	// 初始化默认属性值
	InitHealth(100.f);
	InitMaxHealth(100.f);
}

void UNPCAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// 确保属性值在合理范围内
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
}

void UNPCAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);


	// 处理属性变化后的逻辑
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));

		if (GetHealth() <= 0.f)
		{
			// TODO: 处理角色死亡
		}
	}
}