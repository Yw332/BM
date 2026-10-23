// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ES/BMAnimationTypes.h"
#include "Player/Character/BMPlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "BMAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BM_API UBMAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(BlueprintThreadSafe), Category=Animation)
	ERootMotionMode::Type GetRootMotionMode() const;

protected:
	UPROPERTY(BlueprintReadOnly, Category=Animation)
	ABMPlayerCharacter* OwnerCharacter;
};
