// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chooser.h"
#include "Animation/AnimInstance.h"
#include "ES/BMAnimationTypes.h"
#include "ES/BMPlayerTypes.h"
#include "Player/Character/BMPlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "BMAnimLayerInstance.generated.h"

class UBlendSpace1D;
class UBMDebugAnimation;
class UBMAnimInstance;
/**
 * 
 */
UCLASS()
class BM_API UBMAnimLayerInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativePostEvaluateAnimation() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(BlueprintThreadSafe), Category=Animation)
	UAnimSequence* GetAnimSequenceFromChooserTable(UChooserTable* ChooserTable);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(BlueprintThreadSafe), Category=Animation)
	UBlendSpace1D* GetBlendSpaceFromChooserTable(UChooserTable* ChooserTable);
	
protected:
	virtual void UpdateCharacterTransform(float DeltaSeconds);
	virtual void UpdateCharacterTurn(float DeltaSeconds);
	virtual void UpdateCharacterVelocity(float DeltaSeconds);
	virtual void UpdateCharacterAcceleration(float DeltaSeconds);
	virtual void UpdateCharacterLocomotion(float DeltaSeconds);
	virtual void UpdateCharacterSync(float DeltaSeconds);
	virtual EBMLocoDirection UpdateCharacterLocoDirection(EBMLocoDirection CurLocoDirection, const float SpeedAngle);
	
protected:
	UPROPERTY(BlueprintReadOnly)
	ABMPlayerCharacter* OwnerCharacter = nullptr;
	UPROPERTY()
	UBMAnimStateComponent* AnimStateComponent = nullptr;
	UPROPERTY()
	UAnimInstance* MainAnimInstance = nullptr;

	UPROPERTY(BlueprintReadOnly, Category="State")
	EBMCharacterGate CharacterGate = EBMCharacterGate::Walk;

	UPROPERTY(BlueprintReadWrite, Category="State")
	bool bIsRootMotionActive = false;

	// ~ Transform
	FVector LastCharacterLocation = FVector::ZeroVector;
	FVector CharacterLocation = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly, Category="Transform")
	float CharacterDeltaDistance = 0.0f;
	
	UPROPERTY(BlueprintReadOnly, Category="Transform")
	FRotator CharacterRotation = FRotator::ZeroRotator;

	UPROPERTY(BlueprintReadOnly, Category="Transform")
	FRotator ControlRotation = FRotator::ZeroRotator;
	// ~ End Transform

	// ~ Sync
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Sync")
	FName SyncGroupName = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Sync")
	FName NextMarkerName = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Sync")
	bool bIsLeftFoot = false;
	// ~ End Sync
	
	// ~ Speed
	UPROPERTY(BlueprintReadOnly, Category="Velocity")
	float CharacterSpeed = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category="Velocity")
	FVector CharacterVelocity = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category="Velocity")
	FVector CharacterVelocity2D = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category="Velocity")
	float CharacterVelocityAngle = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category="Velocity")
	EBMLocoDirection CharacterLocoDirection = EBMLocoDirection::Forward;
	// ~ End Speed

	// ~ Acceleration
	UPROPERTY(BlueprintReadOnly, Category="Acceleration")
	float CharacterAccelerationSize = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category="Acceleration")
	FVector CharacterAcceleration = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category="Acceleration")
	FVector CharacterAcceleration2D = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category="Acceleration")
	float CharacterAccelerationAngle = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category="Acceleration")
	EBMLocoDirection CharacterAccelerationDirection = EBMLocoDirection::Forward;
	// ~ End Acceleration

	// ~ Turn
	UPROPERTY(BlueprintReadWrite, Category="State")
	EBMRootYawOffsetMode RootYawOffsetMode = EBMRootYawOffsetMode::Accumulate;
	
	UPROPERTY(BlueprintReadOnly, Category="Turn")
	float RootYawOffset = 0.0f;
	
	UPROPERTY(BlueprintReadOnly, Category="Turn")
	EBMTurnType CharacterTurnType = EBMTurnType::None;

	FFloatSpringState RootYawOffsetSpring;
	// ~ End Turn

	// ~ Locomotion

	UPROPERTY(BlueprintReadOnly, Category="Locomotion")
	FVector CharacterStopLocation = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category="Locomotion")
	FVector CharacterPivotLocation = FVector::ZeroVector;
	// ~ End Locomotion

	// ~ Animation Chooser Tables
	// These tables are used to choose animations based on the current state of the character.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AnimationChooserTable")
	TObjectPtr<UChooserTable> IdleChooserTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AnimationChooserTable")
	TObjectPtr<UChooserTable> TurnChooserTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AnimationChooserTable")
	TObjectPtr<UChooserTable> StartChooserTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AnimationChooserTable")
	TObjectPtr<UChooserTable> LoopChooserTable;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AnimationChooserTable")
	TObjectPtr<UChooserTable> StopChooserTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AnimationChooserTable")
	TObjectPtr<UChooserTable> JumpChooserTable;
	
	UPROPERTY()
	UBMDebugAnimation* DebugAnimation;


	// ~ Settings
	UPROPERTY(BlueprintReadOnly, Category="Setting")
	float ForwardDirectionMaxAngle = 45.f;
	
	UPROPERTY(BlueprintReadOnly, Category="Setting")
	float ForwardDirectionMinAngle = -45.f;

	UPROPERTY(BlueprintReadOnly, Category="Setting")
	float BackwardDirectionMaxAngle = 135.f;

	UPROPERTY(BlueprintReadOnly, Category="Setting")
	float BackwardDirectionMinAngle = -135.f;
	
	UPROPERTY(BlueprintReadOnly, Category="Setting")
	float CharacterDeadZone = 20.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Turn")
	float TurnBackAngle = 160.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Turn")
	float TurnFrontAngle = 90.0f;
	// ~ End Settings
private:
	friend class UBMDebugAnimation;
};
