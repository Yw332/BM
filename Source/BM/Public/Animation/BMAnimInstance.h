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

	/** 角色当前速度（世界空间）。Locomotion 状态机用它判断走/停。 */
	UPROPERTY(BlueprintReadOnly, Transient, Category=Animation)
	FVector Velocity = FVector::ZeroVector;

	/** 角色水平速度大小（cm/s）。Idle/Start/Loop/Stop 之间的切换门限读它。 */
	UPROPERTY(BlueprintReadOnly, Transient, Category=Animation)
	float GroundSpeed = 0.f;

	/** 速度是否足以进入移动循环。带滞回，避免在门限附近来回抖。 */
	UPROPERTY(BlueprintReadOnly, Transient, Category=Animation)
	bool bShouldMove = false;

	/** 进入移动所需的水平速度（cm/s）。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Animation)
	float MoveEnterSpeed = 10.f;

	/** 退出移动所需的速度上限（cm/s），应低于 MoveEnterSpeed 以形成滞回。 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Animation)
	float MoveExitSpeed = 4.f;
};
