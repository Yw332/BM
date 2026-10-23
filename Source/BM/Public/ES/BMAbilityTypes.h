#pragma once

#include "CoreMinimal.h"
#include "BMAbilityTypes.generated.h"

UENUM(BlueprintType)
enum class EBMCombatType : uint8
{
	StandStick = 0,
	SplitStick = 1,
	PokeStick =  2,
};


UENUM(BlueprintType)
enum  class EBMChargeLevel : uint8
{
	Start = 0,
	LevelUp = 1,
	Loop = 2,
	Complete = 3
};

USTRUCT(BlueprintType)
struct FBMChargeMontageData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 StickMomentumLevel = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EBMChargeLevel ChargeLevel = EBMChargeLevel::Start;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EBMCombatType CombatType = EBMCombatType::StandStick;
};
