// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/BMAbilitySystemComponent.h"
#include "AbilitySystem/BMAttributeSet.h"
#include "AbilitySystem/GameplayEffect/BMGameplayTagEffect.h"
#include "ES/BMGameplayTags.h"
#include "Net/UnrealNetwork.h"


void UBMAbilitySystemComponent::AbilityActorInfoSet()
{
	
}

void UBMAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	
}

void UBMAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	for (auto& Ability : DefaultAbilities)
	{
		AddAbilityByClass(Ability);
	}

	SetCombatType(0);
}

void UBMAbilitySystemComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	FGameplayTagContainer Owned;
	GetOwnedGameplayTags(Owned); 
	// UE_LOG(LogTemp, Warning, TEXT("Owned Tags: %s"), *Owned.ToString());

	const UBMAttributeSet* TmpAS = GetSet<UBMAttributeSet>();
	if (TmpAS)
	{
		UE_LOG(LogTemp, Warning, TEXT("Health Base Value : %f"), TmpAS->GetHealthAttribute().GetGameplayAttributeData(TmpAS)->GetBaseValue());
		UE_LOG(LogTemp, Warning, TEXT("Health Current Value : %f"), TmpAS->GetHealthAttribute().GetGameplayAttributeData(TmpAS)->GetCurrentValue());
		// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Stick Momentum: %f"), TmpAS->GetStickMomentum()));
		// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Stick Momentum Level: %f"), TmpAS->GetStickMomentumLevel()));
	}
}

void UBMAbilitySystemComponent::SetPreInputTag(FGameplayTag InPreInputTag)
{
	if (PreInputTag == InPreInputTag) return;

	FGameplayTag CheckTag = FBMGameplayTags::Get().Ability_PreInputWindow;
	if (!HasMatchingGameplayTag(CheckTag)) return;

	PreInputTag = InPreInputTag;
	
	if (GetOwnerRole() != ROLE_Authority)
	{
		Server_PreInputTag(InPreInputTag);
	}
}

void UBMAbilitySystemComponent::ChangeCombatType()
{
	int32 NextCombatType = (static_cast<int32>(CombatType) + 1) % 3;
    SetCombatType(NextCombatType);
}

void UBMAbilitySystemComponent::SetCombatType(int32 InCombatType)
{
	EBMCombatType TmpCombatType = static_cast<EBMCombatType>(InCombatType);
	if (CombatType == TmpCombatType) return;

	CombatType = TmpCombatType;
	if (GetOwnerRole() != ROLE_Authority)
	{
		Server_CombatType(InCombatType);
	}
	OnRep_CombatType();
}

void UBMAbilitySystemComponent::ChargeStickMomentumStart()
{
	if (GetOwnerRole() != ROLE_Authority) return; 
	FGameplayEffectContextHandle Context = MakeEffectContext();
	FGameplayEffectSpecHandle Spec = MakeOutgoingSpec(ChargeStickMomentumEffectClass, 1.f, Context);
	ChargeStickMomentumEffectHandle = ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
}

void UBMAbilitySystemComponent::ChargeStickMomentumComplete()
{
	if (GetOwnerRole() != ROLE_Authority) return; 
	RemoveActiveGameplayEffect(ChargeStickMomentumEffectHandle, -1);
}

void UBMAbilitySystemComponent::OnStickMomentumLevelUp()
{
	FGameplayEffectContextHandle Context = MakeEffectContext();
	FGameplayEffectSpecHandle Spec = MakeOutgoingSpec(ChargeStickMomentumLevelUpEffectClass, 1.f, Context);
	ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	
	OnStickMomentumLevelUpDelegate.Broadcast();
}

void UBMAbilitySystemComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UBMAbilitySystemComponent, PreInputTag, COND_OwnerOnly);
}

void UBMAbilitySystemComponent::OnRep_CombatType()
{
	FGameplayTag CombatTypeTag;

	switch (CombatType)
	{
		case  EBMCombatType::StandStick:
			CombatTypeTag = FBMGameplayTags::Get().State_StickType_Stand;
			break;
		case  EBMCombatType::PokeStick:
			CombatTypeTag = FBMGameplayTags::Get().State_StickType_Poke;
			break;
		case  EBMCombatType::SplitStick:
			CombatTypeTag = FBMGameplayTags::Get().State_StickType_Split;
			break;
		default:
			CombatTypeTag = FBMGameplayTags::Get().State_StickType_Poke;
			break;
	}

	OnStickTypeChanged.Broadcast((int32)CombatType);
	
	FGameplayEffectContextHandle Context = MakeEffectContext();
	FGameplayEffectSpecHandle Spec = MakeOutgoingSpec(CombatTypeEffectClass, 1.f, Context);
	FGameplayEffectSpec* EffectSpecPtr = Spec.Data.Get();
	EffectSpecPtr->DynamicGrantedTags.AddTag(CombatTypeTag);

	RemoveActiveGameplayEffect(CombatTypeEffectHandle, -1);
	CombatTypeEffectHandle = ApplyGameplayEffectSpecToSelf(*EffectSpecPtr);
}

void UBMAbilitySystemComponent::Server_CombatType_Implementation(int32 InCombatType)
{
	SetCombatType(InCombatType);
}

void UBMAbilitySystemComponent::Server_PreInputTag_Implementation(FGameplayTag InPreInputTag)
{
	SetPreInputTag(InPreInputTag);
}

