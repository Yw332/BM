
#pragma once

#include "CoreMinimal.h"
#include "BMAnimationTypes.generated.h"


UENUM(BlueprintType)
enum class EBMSMStage : uint8
{
    Idle = 0,
    Start = 1,
    Loop = 2,
    Stop = 3,
    Pivot = 4,
    Jump = 5,
    Turn = 6,
};


UENUM(BlueprintType)
enum class EBMRootYawOffsetMode : uint8
{
    Accumulate = 0,
    BlendOut =1,
};

UENUM(BlueprintType)
enum class EBMTurnType : uint8
{
    None = 0,
    Left = 1,
    Right = 2,
    LeftToBack = 3,
    RightToBack = 4,
};


UENUM(BlueprintType)
enum class EBMLocoDirection : uint8
{
    Forward = 0,
    Backward = 1,
    Left = 2,
    Right = 3,
};


UENUM(BlueprintType)
enum class EDebugAnimIndex : uint8
{
    None = 0,

    // Velocity
    CharacterVelocity = 1,
    CharacterSpeed = 2,

    // Turn
    CharacterTurnType = 21,
    RootYawOffset = 22,

    // State
    CharacterGate = 41,

    // Other
    GlobalTimeDilation = 221,
    
    
};