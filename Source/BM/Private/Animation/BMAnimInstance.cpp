// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/BMAnimInstance.h"

#include "Animation/Debug/AnimDebug.h"
#include "Net/UnrealNetwork.h"

void UBMAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwnerCharacter = Cast<ABMPlayerCharacter>(TryGetPawnOwner());
}

void UBMAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
}

void UBMAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	// OwnerCharacter 在 NativeInitializeAnimation 里只赋值一次；
	// 换 Pawn、重生、或者拿到时 Pawn 还没 Possess 都会让它是空指针，
	// 这里每次重新取一次，拿不到就安全退出（不退的话动画求值会访问非法内存直接崩）。
	if (!OwnerCharacter)
	{
		OwnerCharacter = Cast<ABMPlayerCharacter>(TryGetPawnOwner());
	}

	if (!OwnerCharacter)
	{
		Velocity = FVector::ZeroVector;
		GroundSpeed = 0.f;
		bShouldMove = false;
		return;
	}

	Velocity = OwnerCharacter->GetVelocity();
	GroundSpeed = Velocity.Size2D();

	// 速度阈值带滞回：进入要比退出难，否则在阈值附近每帧来回切状态，
	// 表现就是动画在 Idle 和走之间闪、以及走起来一顿一顿。
	if (bShouldMove)
	{
		bShouldMove = GroundSpeed > MoveExitSpeed;
	}
	else
	{
		bShouldMove = GroundSpeed >= MoveEnterSpeed;
	}
}

void UBMAnimInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

ERootMotionMode::Type UBMAnimInstance::GetRootMotionMode() const
{
	return RootMotionMode;
}

