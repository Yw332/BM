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

}

void UBMAnimInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

ERootMotionMode::Type UBMAnimInstance::GetRootMotionMode() const
{
	return RootMotionMode;
}

