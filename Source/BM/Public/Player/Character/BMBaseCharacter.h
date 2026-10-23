// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "BMBaseCharacter.generated.h"

class UAttributeSet;
class UGameplayEffect;

UCLASS()
class BM_API ABMBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABMBaseCharacter(const FObjectInitializer& ObjectInitializer);
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BM|Attributes")
	TSubclassOf<UGameplayEffect> DefaultAttributes;
	
	virtual void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> EffectClass, float Level) const;
	virtual void InitDefaultAttributes();

	virtual void InitAbilityActorInfo();

};
