// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BMRootMotionComponent.generated.h"


struct FRootMotionSource_TransformDelta;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BM_API UBMRootMotionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBMRootMotionComponent();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "RootMotion")
	void SetRootMotionEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "RootMotion")
	void ApplyFrameRootMotion(const FTransform& RootMotionDelta);

	/** 获取是否启用 */
	UFUNCTION(BlueprintPure, Category = "RootMotion")
	bool IsRootMotionEnabled() const { return bIsEnabled; }

	UFUNCTION(BlueprintCallable, Category = "RootMotion")
	void SetRootMotionDelta(const FTransform& Delta) { RootMotionDelta = Delta; }
	
protected:
	
	/** 在服务器上更新根运动 */
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerApplyRootMotion(const FTransform& RootMotion);

private:
	/** 拥有者Character的引用 */
	UPROPERTY()
	class ABMPlayerCharacter* OwnerCharacter;

	/** 移动组件的引用 */
	UPROPERTY()
	class UCharacterMovementComponent* MovementComponent;

	/** 根运动源的共享指针 */
	TSharedPtr<FRootMotionSource_TransformDelta> RootMotionSource;

	/** 根运动源的实例名称 */
	FTransform RootMotionDelta;

	/** 是否启用根运动 */
	bool bIsEnabled;

	/** 根运动源的实例名称 */
	FName RootMotionSourceName;

	/** 初始化根运动源 */
	void InitializeRootMotionSource();

	/** 清理根运动源 */
	void CleanupRootMotionSource();

	/** 应用根运动到移动组件 */
	void ApplyRootMotionToMovement(const FTransform& RootMotion);
};
