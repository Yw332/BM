// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Component/BMRootMotionComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/BMAnimLayerInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "Player/Component/BMCharacterMovementComponent.h"

UBMRootMotionComponent::UBMRootMotionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickGroup = TG_PrePhysics;

    SetIsReplicatedByDefault(true);
    SetIsReplicated(true);
    
    bIsEnabled = false;
    RootMotionSourceName = FName("CustomFrameRootMotion");
}

void UBMRootMotionComponent::BeginPlay()
{
    Super::BeginPlay();
    
    // 获取Owner引用
    OwnerCharacter = Cast<ABMPlayerCharacter>(GetOwner());
    if (OwnerCharacter)
    {
        MovementComponent = OwnerCharacter->GetCharacterMovement();
        if(MovementComponent)
        {
            MovementComponent->AddTickPrerequisiteComponent(this);
        }
    }
    
    if (!OwnerCharacter || !MovementComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("BMRootMotionComponent: Owner is not a Character or has no MovementComponent!"));
        SetComponentTickEnabled(false);
    }
}

void UBMRootMotionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    CleanupRootMotionSource();
    Super::EndPlay(EndPlayReason);
}

void UBMRootMotionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    if (!bIsEnabled || !OwnerCharacter || !MovementComponent)
    {
        return;
    }
    
    FTransform RootMotion = RootMotionDelta;
    if (!RootMotion.Equals(FTransform::Identity))
    {
        ApplyFrameRootMotion(RootMotion);
    }
    
}

void UBMRootMotionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UBMRootMotionComponent::SetRootMotionEnabled(bool bEnabled)
{
    if (bIsEnabled == bEnabled)
    {
        return;
    }
    
    bIsEnabled = bEnabled;
    
    if (bEnabled)
    {
        InitializeRootMotionSource();
    }
    else
    {
        CleanupRootMotionSource();
    }
}

void UBMRootMotionComponent::ApplyFrameRootMotion(const FTransform& RootMotion)
{
    if (!bIsEnabled || !MovementComponent)
    {
        return;
    }
    
    // 确保根运动源已初始化
    if (!RootMotionSource.IsValid())
    {
        InitializeRootMotionSource();
    }
    
    if (RootMotionSource.IsValid())
    {
        // 根据网络角色处理
        if (OwnerCharacter->GetLocalRole() == ROLE_Authority)
        {
            // 服务器：直接应用
            ApplyRootMotionToMovement(RootMotion);
        }
        else if (OwnerCharacter->GetLocalRole() == ROLE_AutonomousProxy)
        {
            // 本地客户端：预测并发送到服务器
            ApplyRootMotionToMovement(RootMotion);
            ServerApplyRootMotion(RootMotion);
        }
        // SimulatedProxy会通过RootMotionSource自动同步
    }
}


void UBMRootMotionComponent::InitializeRootMotionSource()
{
    if (!MovementComponent || RootMotionSource.IsValid())
    {
        return;
    }
    
    // 创建根运动源
    RootMotionSource = MakeShareable(new FRootMotionSource_TransformDelta());
    RootMotionSource->InstanceName = RootMotionSourceName;
    RootMotionSource->SetTime(0.f);
    
    // 应用到移动组件
    MovementComponent->ApplyRootMotionSource(RootMotionSource);
    
    UE_LOG(LogTemp, Log, TEXT("BMRootMotionComponent: Initialized RootMotionSource"));
}

void UBMRootMotionComponent::CleanupRootMotionSource()
{
    if (MovementComponent && RootMotionSource.IsValid())
    {
        MovementComponent->RemoveRootMotionSource(RootMotionSourceName);
        RootMotionSource.Reset();
        
        UE_LOG(LogTemp, Log, TEXT("RootMotionComponent: Cleaned up RootMotionSource"));
    }
}

void UBMRootMotionComponent::ApplyRootMotionToMovement(const FTransform& RootMotion)
{
    if (RootMotionSource.IsValid())
    {
        // 更新根运动源的当前帧数据
        RootMotionSource->SetFrameRootMotion(RootMotion);
        
        // 如果根运动源不在活动列表中，重新添加
        if (!MovementComponent->HasRootMotionSources())
        {
            MovementComponent->ApplyRootMotionSource(RootMotionSource);
        }
    }
}

void UBMRootMotionComponent::ServerApplyRootMotion_Implementation(const FTransform& RootMotion)
{
    if (bIsEnabled && MovementComponent)
    {
        ApplyRootMotionToMovement(RootMotion);
    }
}

bool UBMRootMotionComponent::ServerApplyRootMotion_Validate(const FTransform& RootMotion)
{
    // 添加验证逻辑，防止作弊
    // 例如：检查移动距离是否合理
    float MovementDistance = RootMotion.GetLocation().Size();
    const float MaxAllowedDistance = 1000.0f; // 10米，根据你的需求调整
    
    if (MovementDistance > MaxAllowedDistance)
    {
        UE_LOG(LogTemp, Warning, TEXT("RootMotionComponent: Invalid root motion distance: %f"), MovementDistance);
        return false;
    }
    
    return true;
}