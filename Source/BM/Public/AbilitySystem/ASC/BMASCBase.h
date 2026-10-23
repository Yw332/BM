// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BMASCBase.generated.h"


UCLASS()
class BM_API UBMASCBase : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBMASCBase();

	void AddAbilityByClass(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level = 1);
	void RemoveAbilityByClass(TSubclassOf<UGameplayAbility> AbilityClass);
	void RemoveAbilityByTag(FGameplayTag AbilityTag);

	void ApplyEffectByClass(TSubclassOf<UGameplayEffect> EffectClass);
	void RemoveEffectByClass(TSubclassOf<UGameplayEffect> EffectClass);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction) override;
};
