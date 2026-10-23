// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "NPCAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class BM_API UNPCAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UNPCAttributeSet();

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	// ~ Vitality Attributes
	ATTRIBUTE_ACCESSORS(UNPCAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UNPCAttributeSet, MaxHealth);
	// ~ End Vitality Attributes

	
protected:
	
	// ~ Vitality Attributes
	UPROPERTY(BlueprintReadOnly, Category= "Vitality Attributes")
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, Category= "Vitality Attributes")
	FGameplayAttributeData MaxHealth;
};
