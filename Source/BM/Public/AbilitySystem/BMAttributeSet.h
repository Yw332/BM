// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BMAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 用于处理 GameplayEffect 回调数据的辅助结构
 */
USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties(){}

	FGameplayEffectContextHandle EffectContextHandle;
	
	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
};

/**
 * BM 项目的属性集合类
 * 包含角色的所有基础属性，参考黑神话悟空的属性设计
 */
UCLASS()
class BM_API UBMAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UBMAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	// ~ Vitality Attributes
	ATTRIBUTE_ACCESSORS(UBMAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UBMAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UBMAttributeSet, Stamina);
	ATTRIBUTE_ACCESSORS(UBMAttributeSet, MaxStamina);
	ATTRIBUTE_ACCESSORS(UBMAttributeSet, StaminaRegeneration);
	// ~ End Vitality Attributes

	// ~ Combat Attributes
	ATTRIBUTE_ACCESSORS(UBMAttributeSet, AttackDamage);
	ATTRIBUTE_ACCESSORS(UBMAttributeSet, DefenseDamage);
	ATTRIBUTE_ACCESSORS(UBMAttributeSet, CriticalChance);
	ATTRIBUTE_ACCESSORS(UBMAttributeSet, CriticalDamage);
	ATTRIBUTE_ACCESSORS(UBMAttributeSet, Resistance);
	// ~ End Combat Attributes

	// ~ Special Attributes
	ATTRIBUTE_ACCESSORS(UBMAttributeSet, StickMomentum);
	ATTRIBUTE_ACCESSORS(UBMAttributeSet, MaxStickMomentum);
	ATTRIBUTE_ACCESSORS(UBMAttributeSet, StickMomentumDecay);
	ATTRIBUTE_ACCESSORS(UBMAttributeSet, StickMomentumGain);
	ATTRIBUTE_ACCESSORS(UBMAttributeSet, StickMomentumLevel);
	ATTRIBUTE_ACCESSORS(UBMAttributeSet, MaxStickMomentumLevel);
	// ~ End Special Attributes

	
protected:
	
	// ~ Vitality Attributes
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_HealthChanged, Category= "Vitality Attributes")
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxHealthChanged, Category= "Vitality Attributes")
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_StaminaChanged, Category= "Vitality Attributes")
	FGameplayAttributeData Stamina;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_StaminaChanged, Category= "Vitality Attributes")
	FGameplayAttributeData MaxStamina;

	/** 体力回复速度 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_StaminaRegenerationChanged, Category= "Vitality Attributes")
	FGameplayAttributeData StaminaRegeneration;

	// ~ End Vitality Attributes

	// ~ Combat Attributes
	/** 基础攻击力，最后伤害通过公式进行计算 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_AttackDamageChanged, category = "Combat Attributes")
	FGameplayAttributeData AttackDamage;

	/** 基础防御里，表现为减伤 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_DefenseDamageChanged, category = "Combat Attributes")
	FGameplayAttributeData DefenseDamage;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CriticalChanceChanged, category = "Combat Attributes")
	FGameplayAttributeData CriticalChance;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CriticalDamageChanged, category = "Combat Attributes")
	FGameplayAttributeData CriticalDamage;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_ResistanceChanged, category = "Combat Attributes")
	FGameplayAttributeData Resistance;
	// ~ End Combat Attributes

	// ~ Special Attributes
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_StickMomentumChanged, Category= "Special Attributes")
	FGameplayAttributeData StickMomentum;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxStickMomentumChanged, Category= "Special Attributes")
	FGameplayAttributeData MaxStickMomentum;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_StickMomentumGainChanged,  Category= "Special Attributes")
	FGameplayAttributeData StickMomentumGain;      // 棍势获取倍率

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_StickMomentumDecayChanged, Category= "Special Attributes")
	FGameplayAttributeData StickMomentumDecay;     // 棍势衰减速度

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_StickMomentumLevelChanged, Category= "Special Attributes")
	FGameplayAttributeData StickMomentumLevel;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxStickMomentumLevelChanged, Category= "Special Attributes")
	FGameplayAttributeData MaxStickMomentumLevel;
	// ~ End Special Attributes

	// ~ Vitality Attributes Rep Notifies
	UFUNCTION()
	void OnRep_HealthChanged(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_MaxHealthChanged(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_StaminaChanged(const FGameplayAttributeData& OldStamina) const;

	UFUNCTION()
	void OnRep_MaxStaminaChanged(const FGameplayAttributeData& OldMaxStamina) const;

	UFUNCTION()
	void OnRep_StaminaRegenerationChanged(const FGameplayAttributeData& OldStaminaRegeneration) const;

	// ~ Combat Attributes Rep Notifies
	UFUNCTION()
	void OnRep_AttackDamageChanged(const FGameplayAttributeData& OldAttackDamage) const;

	UFUNCTION()
	void OnRep_DefenseDamageChanged(const FGameplayAttributeData& OldDefenseDamage) const;

	UFUNCTION()
	void OnRep_CriticalChanceChanged(const FGameplayAttributeData& OldCriticalChance) const;

	UFUNCTION()
	void OnRep_CriticalDamageChanged(const FGameplayAttributeData& OldCriticalDamage) const;

	UFUNCTION()
	void OnRep_ResistanceChanged(const FGameplayAttributeData& OldResistance) const;

	// ~ Special Attributes Rep Notifies
	UFUNCTION()
	void OnRep_StickMomentumChanged(const FGameplayAttributeData& OldStickMomentum) const;

	UFUNCTION()
	void OnRep_MaxStickMomentumChanged(const FGameplayAttributeData& OldMaxStickMomentum) const;

	UFUNCTION()
	void OnRep_StickMomentumGainChanged(const FGameplayAttributeData& OldStickMomentumGain) const;

	UFUNCTION()
	void OnRep_StickMomentumDecayChanged(const FGameplayAttributeData& OldStickMomentumDecay) const;

	UFUNCTION()
	void OnRep_StickMomentumLevelChanged(const FGameplayAttributeData& OldStickMomentumLevel) const;

	UFUNCTION()
	void OnRep_MaxStickMomentumLevelChanged(const FGameplayAttributeData& OldMaxStickMomentumLevel) const;

private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;
};
