// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ASC/BMASCBase.h"
#include "ES/BMAbilityTypes.h"
#include "BMAbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStickMomentumLevelUp);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStickTypeChangedSignature, int32);

/**
 * 
 */
UCLASS()
class BM_API UBMAbilitySystemComponent : public UBMASCBase
{
	GENERATED_BODY()

public:
	void AbilityActorInfoSet();
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void SetPreInputTag(FGameplayTag InPreInputTag);
	FGameplayTag GetPreInputTag() const { return PreInputTag;};

	virtual void ChangeCombatType();
	virtual void SetCombatType(int32 InCombatType);
	EBMCombatType GetCombatType() const { return CombatType;};	
	
	void ChargeStickMomentumStart();
	void ChargeStickMomentumComplete();
	void OnStickMomentumLevelUp();

	FOnStickTypeChangedSignature OnStickTypeChanged;
	
protected:

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Reliable, Server)
	void Server_PreInputTag(FGameplayTag InPreInputTag);

	UFUNCTION(Reliable, Server)
	void Server_CombatType(int32 InCombatType);
	UFUNCTION()
	void OnRep_CombatType();

public:
	UPROPERTY(BlueprintAssignable, Category = "BM")
	FOnStickMomentumLevelUp OnStickMomentumLevelUpDelegate;
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BM")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category= "BM")
	FGameplayTag PreInputTag;

	UPROPERTY(ReplicatedUsing=OnRep_CombatType, BlueprintReadOnly, Category = "BM")
	EBMCombatType CombatType =  EBMCombatType::PokeStick;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BM")
	TSubclassOf<UGameplayEffect> CombatTypeEffectClass;

	FActiveGameplayEffectHandle CombatTypeEffectHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BM")
	TSubclassOf<UGameplayEffect> ChargeStickMomentumEffectClass;

	FActiveGameplayEffectHandle ChargeStickMomentumEffectHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BM")
	TSubclassOf<UGameplayEffect> ChargeStickMomentumLevelUpEffectClass;

};
