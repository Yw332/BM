// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ASC/BMASCBase.h"


// Sets default values for this component's properties
UBMASCBase::UBMASCBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBMASCBase::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBMASCBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}



void UBMASCBase::RemoveAbilityByClass(TSubclassOf<UGameplayAbility> AbilityClass)
{
	// 查找并移除所有该类型的能力
	TArray<FGameplayAbilitySpec*> AbilitiesToRemove;
	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (Spec.Ability && Spec.Ability->GetClass() == AbilityClass)
		{
			AbilitiesToRemove.Add(&Spec);
		}
	}
    
	for (FGameplayAbilitySpec* Spec : AbilitiesToRemove)
	{
		ClearAbility(Spec->Handle);
	}
}

void UBMASCBase::RemoveAbilityByTag(FGameplayTag AbilityTag)
{
	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
    
	for (const FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (Spec.Ability && Spec.Ability->AbilityTags.HasTag(AbilityTag))
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}
    
	for (const FGameplayAbilitySpecHandle& Handle : AbilitiesToRemove)
	{
		ClearAbility(Handle);
	}
}

void UBMASCBase::ApplyEffectByClass(TSubclassOf<UGameplayEffect> EffectClass)
{
	FGameplayEffectContextHandle EffectContext = MakeEffectContext();
	EffectContext.AddSourceObject(this);
	FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingSpec(EffectClass, 1, EffectContext);
	ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}

void UBMASCBase::RemoveEffectByClass(TSubclassOf<UGameplayEffect> EffectClass)
{
	TArray<FActiveGameplayEffectHandle> HandlesToRemove;

	FGameplayEffectQuery EffectQuery;
	EffectQuery.EffectDefinition = EffectClass;

	HandlesToRemove = GetActiveEffects(EffectQuery);

	for (const FActiveGameplayEffectHandle& Handle : HandlesToRemove)
	{
		RemoveActiveGameplayEffect(Handle);
	}
}

void UBMASCBase::AddAbilityByClass(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level)
{
	FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, Level);
	GiveAbility(AbilitySpec);
}
