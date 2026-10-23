// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/RootMotionSource.h"
#include "BMCharacterMovementComponent.generated.h"

/** 
 * MoveToDynamicForce moves the target to a given location in world space over the duration, where the end location
 * is dynamic and can change during the move (meant to be used for things like moving to a moving target)
 */
 
USTRUCT()
struct FRootMotionSource_TransformDelta : public FRootMotionSource
{
	GENERATED_USTRUCT_BODY()

	FRootMotionSource_TransformDelta();

	virtual ~FRootMotionSource_TransformDelta() {}

	/** 目标位置 */
	UPROPERTY()
	FTransform CurrentFrameRootMotion;

	/** 是否有待处理的根运动 */
	UPROPERTY()
	bool bHasPendingRootMotion;
	
	/** 每帧调用此函数来更新根运动 */
	void SetFrameRootMotion(const FTransform& InRootMotion);

	virtual FRootMotionSource* Clone() const override;

	virtual bool Matches(const FRootMotionSource* Other) const override;

	virtual void PrepareRootMotion(
		float SimulationTime, 
		float MovementTickTime,
		const ACharacter& Character, 
		const UCharacterMovementComponent& MoveComponent
		) override;

	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;

	virtual UScriptStruct* GetScriptStruct() const override;

	virtual FString ToSimpleString() const override;

	void AddReferencedObjects(class FReferenceCollector& Collector) override;
};

template<>
struct TStructOpsTypeTraits< FRootMotionSource_TransformDelta > : public TStructOpsTypeTraitsBase2< FRootMotionSource_TransformDelta >
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BM_API UBMCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UBMCharacterMovementComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
