// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/BMAnimLayerInstance.h"

#include "AnimCharacterMovementLibrary.h"
#include "Player/Component/BMAnimStateComponent.h"
#include "Animation/BMAnimInstance.h"
#include "ChooserFunctionLibrary.h"
#include "KismetAnimationLibrary.h"
#include "Animation/BlendSpace1D.h"
#include "Animation/Debug/AnimDebug.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/Component/BMRootMotionComponent.h"

void UBMAnimLayerInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	DebugAnimation = NewObject<UBMDebugAnimation>(this);
	
	OwnerCharacter = Cast<ABMPlayerCharacter, APawn>(TryGetPawnOwner());
	if (!IsValid(OwnerCharacter)) return;
	AnimStateComponent = OwnerCharacter->FindComponentByClass<UBMAnimStateComponent>();
	if (!IsValid(AnimStateComponent)) return;
	MainAnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	if (!IsValid(MainAnimInstance)) return;
}

void UBMAnimLayerInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!IsValid(OwnerCharacter))
	{
		OwnerCharacter = Cast<ABMPlayerCharacter, APawn>(TryGetPawnOwner());
		if (!IsValid(OwnerCharacter)) return;
	};
	if (!IsValid(AnimStateComponent))
	{
		AnimStateComponent = OwnerCharacter->FindComponentByClass<UBMAnimStateComponent>();
		if (!IsValid(AnimStateComponent)) return;
	};

	if(bIsRootMotionActive)
	{
		MainAnimInstance->SetRootMotionMode(ERootMotionMode::Type::RootMotionFromEverything);
		SetRootMotionMode(ERootMotionMode::Type::RootMotionFromEverything);
	}else
	{
		MainAnimInstance->SetRootMotionMode(ERootMotionMode::Type::RootMotionFromMontagesOnly);
		SetRootMotionMode(ERootMotionMode::Type::RootMotionFromMontagesOnly);
	}
	
	
	UpdateCharacterTurn(DeltaSeconds);

	DebugAnimation->DebugFunc(this);
}

void UBMAnimLayerInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
	if (!IsValid(OwnerCharacter))
	{
		OwnerCharacter = Cast<ABMPlayerCharacter, APawn>(TryGetPawnOwner());
		if (!IsValid(OwnerCharacter)) return;
	};
	if (!IsValid(AnimStateComponent))
	{
		AnimStateComponent = OwnerCharacter->FindComponentByClass<UBMAnimStateComponent>();
		if (!IsValid(AnimStateComponent)) return;
	};
	
	if (!IsValid(OwnerCharacter) || !IsValid(AnimStateComponent)) return;

	CharacterGate = OwnerCharacter->GetCharacterGate();
	ControlRotation = AnimStateComponent->ControlRotation;
	CharacterRotation = OwnerCharacter->GetActorRotation();
	
	UpdateCharacterTransform(DeltaSeconds);
	UpdateCharacterVelocity(DeltaSeconds);
	UpdateCharacterLocomotion(DeltaSeconds);
	UpdateCharacterTurn(DeltaSeconds);
}

void UBMAnimLayerInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UBMAnimLayerInstance::UpdateCharacterTurn(float DeltaSeconds)
{
	if(RootYawOffsetMode == EBMRootYawOffsetMode::Accumulate)
	{
		float Offset = OwnerCharacter->GetActorRotation().Yaw - ControlRotation.Yaw;
		RootYawOffset = UKismetMathLibrary::NormalizeAxis(Offset); 
	}
	else if(RootYawOffsetMode == EBMRootYawOffsetMode::BlendOut)
	{
		float Tmp = UKismetMathLibrary::FloatSpringInterp(RootYawOffset, 0.f, RootYawOffsetSpring,
			80,1, DeltaSeconds,1,0.5);
		RootYawOffset = UKismetMathLibrary::NormalizeAxis(Tmp);
	}

	CharacterTurnType = EBMTurnType::None;
	if(RootYawOffset > TurnBackAngle)
	{
		CharacterTurnType = EBMTurnType::LeftToBack;
	}
	else if(RootYawOffset > TurnFrontAngle)
	{
		CharacterTurnType = EBMTurnType::Left;
	}
	else if(RootYawOffset < -TurnBackAngle)
	{
		CharacterTurnType = EBMTurnType::RightToBack;
	}
	else if(RootYawOffset < -TurnFrontAngle)
	{
		CharacterTurnType = EBMTurnType::Right;
	}
	
}

void UBMAnimLayerInstance::UpdateCharacterVelocity(float DeltaSeconds)
{
	// ~ Speed
	CharacterVelocity = OwnerCharacter->GetVelocity();
	CharacterVelocity2D = FVector(CharacterVelocity.X,CharacterVelocity.Y, 0.f);
	// UE_LOG(LogTemp, Warning, TEXT("CharacterVelocity2D: %s"), *CharacterVelocity2D.ToString());
	CharacterSpeed = CharacterVelocity2D.Size2D();
	// UE_LOG(LogTemp, Warning, TEXT("CharacterSpeed: %f"), CharacterSpeed);
	CharacterVelocityAngle = UKismetAnimationLibrary::CalculateDirection(CharacterVelocity , CharacterRotation);
	
	CharacterLocoDirection = UpdateCharacterLocoDirection(CharacterLocoDirection, CharacterVelocityAngle);
	
	
	// ~ End Speed
}

void UBMAnimLayerInstance::UpdateCharacterAcceleration(float DeltaSeconds)
{
	CharacterAcceleration = OwnerCharacter->GetCharacterMovement()->GetCurrentAcceleration();
	CharacterAcceleration2D = FVector(CharacterAcceleration.X,CharacterAcceleration.Y, 0.f);
	CharacterAccelerationSize = CharacterAcceleration2D.Size2D();

	CharacterAccelerationAngle = UKismetAnimationLibrary::CalculateDirection(CharacterAcceleration , CharacterRotation);

	CharacterAccelerationDirection = UpdateCharacterLocoDirection(CharacterAccelerationDirection, CharacterAccelerationAngle);
}

void UBMAnimLayerInstance::UpdateCharacterLocomotion(float DeltaSeconds)
{
	// Update StopLocation
	UCharacterMovementComponent* CharacterMovement = OwnerCharacter->GetCharacterMovement();
	CharacterStopLocation = UAnimCharacterMovementLibrary::PredictGroundMovementStopLocation(
		CharacterVelocity, CharacterMovement->bUseSeparateBrakingFriction,
		CharacterMovement->BrakingFriction, CharacterMovement->GroundFriction,
		CharacterMovement->BrakingFrictionFactor,CharacterMovement->BrakingDecelerationWalking	);
	CharacterPivotLocation = UAnimCharacterMovementLibrary::PredictGroundMovementPivotLocation(
		CharacterAcceleration, CharacterVelocity, CharacterMovement->GroundFriction);
}

void UBMAnimLayerInstance::UpdateCharacterSync(float DeltaSeconds)
{
	FMarkerSyncAnimPosition SyncData = GetSyncGroupPosition(SyncGroupName);
	if (!SyncData.IsValid()) return;

	NextMarkerName = SyncData.NextMarkerName;
	if (NextMarkerName == "L") bIsLeftFoot = true;
	else bIsLeftFoot = false;
}


EBMLocoDirection UBMAnimLayerInstance::UpdateCharacterLocoDirection(EBMLocoDirection CurLocoDirection, const float SpeedAngle)
{
	// Use DeadZone to avoid jitter
	switch (CurLocoDirection) 
	{
	case EBMLocoDirection::Forward:
		if(SpeedAngle > ForwardDirectionMinAngle - CharacterDeadZone  &&  SpeedAngle <= ForwardDirectionMaxAngle + CharacterDeadZone) 
		{
			return EBMLocoDirection::Forward;
		}
		break;
	case EBMLocoDirection::Backward:
		if(SpeedAngle < BackwardDirectionMinAngle + CharacterDeadZone  ||  SpeedAngle >= BackwardDirectionMaxAngle - CharacterDeadZone) 
		{
			return EBMLocoDirection::Backward;
		}
		break;
	case EBMLocoDirection::Left:
		if(SpeedAngle > BackwardDirectionMinAngle - CharacterDeadZone  &&  SpeedAngle <= ForwardDirectionMinAngle + CharacterDeadZone) 
		{
			return  EBMLocoDirection::Left;
		}
		break;
	case EBMLocoDirection::Right:
		if(SpeedAngle > ForwardDirectionMaxAngle - CharacterDeadZone  &&  SpeedAngle <= BackwardDirectionMaxAngle + CharacterDeadZone) 
		{
			return EBMLocoDirection::Right;
		}
		break;
	default:
		break;
	}

	// Default
	if(SpeedAngle > ForwardDirectionMinAngle  &&  SpeedAngle <= ForwardDirectionMaxAngle)
	{
		return  EBMLocoDirection::Forward;
	}
	else if(SpeedAngle < BackwardDirectionMinAngle  ||  SpeedAngle >= BackwardDirectionMaxAngle)
	{
		return EBMLocoDirection::Backward;
	}
	else if(SpeedAngle < ForwardDirectionMinAngle  &&  SpeedAngle >= BackwardDirectionMinAngle)
	{
		return  EBMLocoDirection::Left;
	}
	else if(SpeedAngle > ForwardDirectionMaxAngle &&  SpeedAngle <= BackwardDirectionMaxAngle)
	{
		return EBMLocoDirection::Right;
	}
	return EBMLocoDirection::Forward;
}

UAnimSequence* UBMAnimLayerInstance::GetAnimSequenceFromChooserTable(UChooserTable* ChooserTable)
{
	UObject* Result = UChooserFunctionLibrary::EvaluateChooser(this,ChooserTable, UAnimSequence::StaticClass());
	if (!IsValid(Result))
	{
		UE_LOG(LogTemp, Warning, TEXT("[AnimDebug]: Chooser Table %s Result is not valid "), *GetNameSafe(ChooserTable));
		return nullptr;
	}
	return Cast<UAnimSequence>(Result);

}

UBlendSpace1D* UBMAnimLayerInstance::GetBlendSpaceFromChooserTable(UChooserTable* ChooserTable)
{
	UObject* Result = UChooserFunctionLibrary::EvaluateChooser(this,ChooserTable, UBlendSpace1D::StaticClass());
	if (!IsValid(Result))
	{
		UE_LOG(LogTemp, Warning, TEXT("[AnimDebug]: Chooser Table %s Result is not valid "), *GetNameSafe(ChooserTable));
		return nullptr;
	}
	return Cast<UBlendSpace1D>(Result);
}

void UBMAnimLayerInstance::UpdateCharacterTransform(float DeltaSeconds)
{
	CharacterLocation = OwnerCharacter->GetActorLocation();
	CharacterDeltaDistance = (CharacterLocation - LastCharacterLocation).Size2D();
	LastCharacterLocation = CharacterLocation;
}

void UBMAnimLayerInstance::NativePostEvaluateAnimation()
{
	Super::NativePostEvaluateAnimation();
}

