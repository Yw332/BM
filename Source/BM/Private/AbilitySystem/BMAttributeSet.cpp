// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/BMAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/BMAbilitySystemComponent.h"

UBMAttributeSet::UBMAttributeSet()
{
	// 初始化默认属性值
	InitHealth(100.f);
	InitMaxHealth(100.f);
	InitStamina(100.f);
	InitMaxStamina(100.f);
	InitStaminaRegeneration(1.f);
	
	InitAttackDamage(10.f);
	InitDefenseDamage(5.f);
	InitCriticalChance(5.f);
	InitCriticalDamage(150.f);
	InitResistance(0.f);
	
	InitStickMomentum(0.f);
	InitMaxStickMomentum(100.f);
	InitStickMomentumGain(0.f);
	InitStickMomentumDecay(0.5f);
	InitStickMomentumLevel(0.f);
	InitMaxStickMomentumLevel(3.f);
}

void UBMAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Vitality Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet, StaminaRegeneration, COND_None, REPNOTIFY_Always);

	// Combat Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet, AttackDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet, DefenseDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet, CriticalChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet, CriticalDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet, Resistance, COND_None, REPNOTIFY_Always);

	// Special Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet, StickMomentum, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet, MaxStickMomentum, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet, StickMomentumGain, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet, StickMomentumDecay, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet, StickMomentumLevel, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBMAttributeSet, MaxStickMomentumLevel, COND_None, REPNOTIFY_Always);
}

void UBMAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// 确保属性值在合理范围内
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	else if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStamina());
	}
	else if (Attribute == GetStickMomentumAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStickMomentum());
	}
	else if (Attribute == GetStickMomentumLevelAttribute())
 	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStickMomentumLevel());
	}
	else if (Attribute == GetCriticalChanceAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 100.f); // 暴击率 0-100%
	}
	else if (Attribute == GetCriticalDamageAttribute())
	{
		NewValue = FMath::Max(NewValue, 100.f); // 暴击伤害最少100%
	}
}

void UBMAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	// 处理属性变化后的逻辑
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));

		if (GetHealth() <= 0.f && Props.TargetCharacter)
		{
			// TODO: 处理角色死亡
		}
	}
	else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.f, GetMaxStamina()));
	}
	else if (Data.EvaluatedData.Attribute == GetStickMomentumAttribute())
	{
		// 确保棍势值不超过最大值
		float ClampedMomentum = FMath::Clamp(GetStickMomentum(), 0.f, GetMaxStickMomentum());
    
		// 检查是否达到最大值且可以升级
		if (GetStickMomentum() >= GetMaxStickMomentum() && GetStickMomentumLevel() < GetMaxStickMomentumLevel())
		{
			if (UBMAbilitySystemComponent* ASC = Cast<UBMAbilitySystemComponent>(GetOwningAbilitySystemComponent()))
			{
				ASC->OnStickMomentumLevelUp();
			}
		}
		else
		{
			SetStickMomentum(ClampedMomentum);
		}
	}
}

void UBMAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	// Source = 效果的发起者，Target = 效果的目标 (拥有此AttributeSet的角色)
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}

// Vitality Attributes Rep Notifies
void UBMAttributeSet::OnRep_HealthChanged(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBMAttributeSet, Health, OldHealth);
}

void UBMAttributeSet::OnRep_MaxHealthChanged(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBMAttributeSet, MaxHealth, OldMaxHealth);
}

void UBMAttributeSet::OnRep_StaminaChanged(const FGameplayAttributeData& OldStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBMAttributeSet, Stamina, OldStamina);
}

void UBMAttributeSet::OnRep_MaxStaminaChanged(const FGameplayAttributeData& OldMaxStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBMAttributeSet, MaxStamina, OldMaxStamina);
}

void UBMAttributeSet::OnRep_StaminaRegenerationChanged(const FGameplayAttributeData& OldStaminaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBMAttributeSet, StaminaRegeneration, OldStaminaRegeneration);
}

// Combat Attributes Rep Notifies
void UBMAttributeSet::OnRep_AttackDamageChanged(const FGameplayAttributeData& OldAttackDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBMAttributeSet, AttackDamage, OldAttackDamage);
}

void UBMAttributeSet::OnRep_DefenseDamageChanged(const FGameplayAttributeData& OldDefenseDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBMAttributeSet, DefenseDamage, OldDefenseDamage);
}

void UBMAttributeSet::OnRep_CriticalChanceChanged(const FGameplayAttributeData& OldCriticalChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBMAttributeSet, CriticalChance, OldCriticalChance);
}

void UBMAttributeSet::OnRep_CriticalDamageChanged(const FGameplayAttributeData& OldCriticalDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBMAttributeSet, CriticalDamage, OldCriticalDamage);
}

void UBMAttributeSet::OnRep_ResistanceChanged(const FGameplayAttributeData& OldResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBMAttributeSet, Resistance, OldResistance);
}

// Special Attributes Rep Notifies
void UBMAttributeSet::OnRep_StickMomentumChanged(const FGameplayAttributeData& OldStickMomentum) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBMAttributeSet, StickMomentum, OldStickMomentum);
}

void UBMAttributeSet::OnRep_MaxStickMomentumChanged(const FGameplayAttributeData& OldMaxStickMomentum) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBMAttributeSet, MaxStickMomentum, OldMaxStickMomentum);
}

void UBMAttributeSet::OnRep_StickMomentumGainChanged(const FGameplayAttributeData& OldStickMomentumGain) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBMAttributeSet, StickMomentumGain, OldStickMomentumGain);
}

void UBMAttributeSet::OnRep_StickMomentumDecayChanged(const FGameplayAttributeData& OldStickMomentumDecay) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBMAttributeSet, StickMomentumDecay, OldStickMomentumDecay);
}

void UBMAttributeSet::OnRep_StickMomentumLevelChanged(const FGameplayAttributeData& OldStickMomentumLevel) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBMAttributeSet, StickMomentumLevel, OldStickMomentumLevel);
}

void UBMAttributeSet::OnRep_MaxStickMomentumLevelChanged(const FGameplayAttributeData& OldMaxStickMomentumLevel) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBMAttributeSet, MaxStickMomentumLevel, OldMaxStickMomentumLevel);
}
