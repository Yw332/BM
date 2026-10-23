// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Component/BMCharacterMovementComponent.h"

#include "Animation/BMAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"

FRootMotionSource_TransformDelta::FRootMotionSource_TransformDelta():
    CurrentFrameRootMotion(FTransform::Identity)
    , bHasPendingRootMotion(false)
{
    // 设置为持续性的源，不会自动结束
    Duration = -1.f;
    bInLocalSpace = false;
    bNeedsSimulatedCatchup = true;
    bSimulatedNeedsSmoothing = true;
    
    // 设置标志
    Settings.SetFlag(ERootMotionSourceSettingsFlags::UseSensitiveLiftoffCheck);
    Settings.SetFlag(ERootMotionSourceSettingsFlags::DisablePartialEndTick);
    
    // 使用Additive模式，完全替代其他根运动
    AccumulateMode = ERootMotionAccumulateMode::Override;
    
    // 设置高优先级
    Priority = 1000;
}

void FRootMotionSource_TransformDelta::SetFrameRootMotion(const FTransform& InRootMotion)
{
    CurrentFrameRootMotion = InRootMotion;
    bHasPendingRootMotion = !InRootMotion.Equals(FTransform::Identity);
}

FRootMotionSource* FRootMotionSource_TransformDelta::Clone() const
{
    FRootMotionSource_TransformDelta* CopyPtr = new FRootMotionSource_TransformDelta(*this);
    return CopyPtr;
}

bool FRootMotionSource_TransformDelta::Matches(const FRootMotionSource* Other) const
{
    if (!Other)
    {
        return false;
    }

    const FRootMotionSource_TransformDelta* OtherCast = static_cast<const FRootMotionSource_TransformDelta*>(Other);

    return InstanceName == OtherCast->InstanceName;
}

void FRootMotionSource_TransformDelta::PrepareRootMotion(
    float SimulationTime,
    float MovementTickTime,
    const ACharacter& Character,
    const UCharacterMovementComponent& MoveComponent)
{
    RootMotionParams.Clear();
    
    // 只在有待处理的根运动时应用
    if (bHasPendingRootMotion && MovementTickTime > 0.f)
    {
        // 应用当前帧的根运动
        RootMotionParams.Set(CurrentFrameRootMotion);
        RootMotionParams.bHasRootMotion = true;
        
        // 清除标记，等待下一帧更新
        bHasPendingRootMotion = false;
        CurrentFrameRootMotion = FTransform::Identity;
    }
}

bool FRootMotionSource_TransformDelta::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
    if (!FRootMotionSource::NetSerialize(Ar, Map, bOutSuccess))
    {
        return false;
    }
    
    // 序列化当前帧的根运动数据
    Ar << CurrentFrameRootMotion;
    Ar << bHasPendingRootMotion;

    bOutSuccess = true;
    return true;
}

UScriptStruct* FRootMotionSource_TransformDelta::GetScriptStruct() const
{
    return FRootMotionSource_TransformDelta::StaticStruct();
}

FString FRootMotionSource_TransformDelta::ToSimpleString() const
{
    return FString::Printf(TEXT("[TransformDelta]"));
}

void FRootMotionSource_TransformDelta::AddReferencedObjects(FReferenceCollector& Collector)
{
    FRootMotionSource::AddReferencedObjects(Collector);
}


UBMCharacterMovementComponent::UBMCharacterMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBMCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UBMCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}