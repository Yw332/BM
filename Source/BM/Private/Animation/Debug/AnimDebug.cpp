
#include "Animation/Debug/AnimDebug.h"
#include "Animation/BMAnimLayerInstance.h"


void UBMDebugAnimation::DebugFunc(UBMAnimLayerInstance* AnimInstance) const
{
	UWorld* World = GetWorld();
	if(!IsValid(World)) return;
	ABMPlayerCharacter *PlayerCharacter = Cast<ABMPlayerCharacter>(AnimInstance->GetOwningActor());
	if(!IsValid(PlayerCharacter)) return;
	
	FString DebugMessage = TEXT("");

	/*if (DebugAnim::bShowCharacterState)
	{
		DebugMessage = FString::Printf(TEXT("Equipped Weapon: %s"), *UEnum::GetValueAsString(CurrentEquippedWeaponType));
		GEngine->AddOnScreenDebugMessage(static_cast<int>(EDebugAnimIndex::CharacterEquippedWeapon), 5.f, FColor::Green, DebugMessage);
		
		DebugMessage = FString::Printf(TEXT("Movement State: %s"), *UEnum::GetValueAsString(CurrentMovementState));
		GEngine->AddOnScreenDebugMessage(static_cast<int>(EDebugAnimIndex::CharacterMovementState), 5.f, FColor::Green, DebugMessage);
	}*/
	// UE_LOG(LogTemp, Warning, TEXT("AnimDebug Params : bShowState: %d , bShowVelocity: %d , bShowTurn %d"), DebugAnim::bShowState, DebugAnim::bShowVelocity, DebugAnim::bShowTurn);
	if (DebugAnim::bShowState)
	{
		DebugMessage = FString::Printf(TEXT("State: %s"), *UEnum::GetValueAsString(AnimInstance->CharacterGate));
		GEngine->AddOnScreenDebugMessage(static_cast<int>(EDebugAnimIndex::CharacterGate), 0.f, FColor::Green, DebugMessage);
	}

	if (DebugAnim::bShowVelocity)
	{
		DebugMessage = FString::Printf(TEXT("Velocity: %s"), *AnimInstance->CharacterVelocity.ToString());
		GEngine->AddOnScreenDebugMessage(static_cast<int>(EDebugAnimIndex::CharacterVelocity), 0.f, FColor::Yellow, DebugMessage);

		DebugMessage = FString::Printf(TEXT("Character Speed: %f"), AnimInstance->CharacterSpeed);
		GEngine->AddOnScreenDebugMessage(static_cast<int>(EDebugAnimIndex::CharacterSpeed), 0.f, FColor::Yellow, DebugMessage);
		
		/*DebugMessage = FString::Printf(TEXT("Velocity Angle: %f"), CharacterVelocityAngle);
		GEngine->AddOnScreenDebugMessage(static_cast<int>(EDebugAnimIndex::CharacterVelocityAngle), 5.f, FColor::Yellow, DebugMessage);
		
		FVector ShowVelocity = CharacterVelocity.GetSafeNormal() * 100.0f;
		FVector StartLocation = FVector(CharacterLocation.X, CharacterLocation.Y, CharacterLocation.Z - PlayerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		FVector EndLocation = StartLocation + ShowVelocity;
		DrawDebugDirectionalArrow(World, StartLocation, EndLocation, 100.0f, FColor::Yellow, false, 0, 0, 5.0f);
		DrawDebugString(World, EndLocation, FString::Printf(TEXT("Velocity")), nullptr, FColor::Yellow, 0.f, false, 1.0f);*/
	}

	/*if (DebugAnim::bShowCharacterJump)
	{
		DebugMessage = FString::Printf(TEXT("Ground Distance: %f"), GroundDistance);
		GEngine->AddOnScreenDebugMessage(static_cast<int>(EDebugAnimIndex::CharacterGroundDistance), 5.f, FColor::Green, DebugMessage);

		DebugMessage = FString::Printf(TEXT("Falling Time: %f"), FallingTime);
		GEngine->AddOnScreenDebugMessage(static_cast<int>(EDebugAnimIndex::CharacterFallingTime), 5.f, FColor::Green, DebugMessage);

		// DrawDebugCapsule(World, CharacterLocation, GroundDistance, PlayerCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius(), FRotator(0, 0, 0), FColor::Green, false, 0.f, 0, 2.0f);
	}

	if (DebugAnim::bShowCharacterAcceleration)
	{
		DebugMessage = FString::Printf(TEXT("Acceleration: %s"), *CharacterAcceleration.ToString());
		GEngine->AddOnScreenDebugMessage(static_cast<int>(EDebugAnimIndex::CharacterAcceleration), 5.f, FColor::Blue, DebugMessage);

		DebugMessage = FString::Printf(TEXT("Acceleration Size: %f"), CharacterAccelerationSize);
		GEngine->AddOnScreenDebugMessage(static_cast<int>(EDebugAnimIndex::CharacterAccelerationSize), 5.f, FColor::Blue, DebugMessage);

		DebugMessage = FString::Printf(TEXT("Acceleration Angle: %f"), CharacterAccelerationAngle);
		GEngine->AddOnScreenDebugMessage(static_cast<int>(EDebugAnimIndex::CharacterAccelerationAngle), 5.f, FColor::Blue, DebugMessage);

		DebugMessage = FString::Printf(TEXT("Acceleration Direction: %s"), *UEnum::GetValueAsString(CharacterAccelerationDirection));
		GEngine->AddOnScreenDebugMessage(static_cast<int>(EDebugAnimIndex::CharacterAccelerationDirection), 5.f, FColor::Blue, DebugMessage);
		
		FVector ShowAcceleration = CharacterAcceleration.GetSafeNormal() * 100.0f;
		FVector StartLocation = FVector(CharacterLocation.X, CharacterLocation.Y, CharacterLocation.Z - PlayerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		FVector EndLocation = StartLocation + ShowAcceleration;
		DrawDebugDirectionalArrow(World, StartLocation, EndLocation, 100.0f, FColor::Blue, false, 0, 0, 5.0f);
		DrawDebugString(World, EndLocation, FString::Printf(TEXT("Acceleration")), nullptr, FColor::Blue, 0.f, false, 1.0f);

		DebugMessage = FString::Printf(TEXT("Pivot Location: %s"), *CharacterPivotLocation.ToString());
		GEngine->AddOnScreenDebugMessage(static_cast<int>(EDebugAnimIndex::CharacterPivotLocation), 5.f, FColor::Red, DebugMessage);
		DrawDebugSphere(World, CharacterLocation + CharacterPivotLocation, 20.0f, 12, FColor::Red, false, 0.f, 0, 2.0f);
		
		UCapsuleComponent* CapsuleComponent = PlayerCharacter->GetCapsuleComponent();
		DebugMessage = FString::Printf(TEXT("Stop Location Distance: %f"), CharacterStopLocation.Size2D());
		GEngine->AddOnScreenDebugMessage(static_cast<int>(EDebugAnimIndex::CharacterStopLocationDistance), 5.f, FColor::Red, DebugMessage);
		
		DebugMessage = FString::Printf(TEXT("Stop Location: %s"), *CharacterStopLocation.ToString());
		DrawDebugCapsule(World, CharacterLocation + CharacterStopLocation, CapsuleComponent->GetScaledCapsuleHalfHeight(),
			CapsuleComponent->GetScaledCapsuleRadius(),FQuat::Identity, FColor::Blue,false, 0, 0, 2.0f);
		DrawDebugSphere(World, StopLocation, 50.0f, 12, FColor::Red, false, -1.0f, 0, 5.0f);
	}*/

	if(DebugAnim::bShowTurn)
	{
		DebugMessage = FString::Printf(TEXT("Turn Type: %s"), *UEnum::GetValueAsString(AnimInstance->CharacterTurnType));
		GEngine->AddOnScreenDebugMessage(static_cast<int>(EDebugAnimIndex::CharacterTurnType), 0.f, FColor::Purple, DebugMessage);

		DebugMessage = FString::Printf(TEXT("Root Yaw Offset: %f"), AnimInstance->RootYawOffset);
		GEngine->AddOnScreenDebugMessage(static_cast<int>(EDebugAnimIndex::RootYawOffset), 0.f, FColor::Purple, DebugMessage);
	}

	/*
	if(DebugAnim::bShowCharacterLocomotion)
	{
		DebugMessage = FString::Printf(TEXT("Locomotion Direction: %s"), *UEnum::GetValueAsString(CharacterLocomotionDirection));
		GEngine->AddOnScreenDebugMessage(static_cast<int>(EDebugAnimIndex::CharacterLocomotionDirection), 5.f, FColor::Red, DebugMessage);

		DebugMessage = FString::Printf(TEXT("Locomotion LeanAngle: %f"), CharacterLeanAngle);
		GEngine->AddOnScreenDebugMessage(static_cast<int>(EDebugAnimIndex::CharacterLeanAngle), 5.f, FColor::Red, DebugMessage);

		DebugMessage = FString::Printf(TEXT("Locomotion DeltaDistance: %f"), CharacterDeltaDistance);
		GEngine->AddOnScreenDebugMessage(static_cast<int>(EDebugAnimIndex::CharacterDeltaDistance), 5.f, FColor::Red, DebugMessage);

		DebugMessage = FString::Printf(TEXT("Root Yaw Offset: %f"), RootYawOffset);
		GEngine->AddOnScreenDebugMessage(static_cast<int>(EDebugAnimIndex::RootYawOffset), 5.f, FColor::Red, DebugMessage);

		DebugMessage = FString::Printf(TEXT("Root Yaw Offset Mode: %s"), *UEnum::GetValueAsString(RootYawOffsetMode));
		GEngine->AddOnScreenDebugMessage(static_cast<int>(EDebugAnimIndex::RootYawOffsetMode), 5.f, FColor::Red, DebugMessage);
	}
	*/
	
	float TimeDilation = World->GetWorldSettings()->TimeDilation;
	if(FMath::Abs(TimeDilation - 1.f) >= SMALL_NUMBER)
	{
		GEngine->AddOnScreenDebugMessage(static_cast<int>(EDebugAnimIndex::GlobalTimeDilation), 0.f, FColor::Red, FString::Printf(TEXT("Time Dilation: %f"), TimeDilation));
	}
	
}
