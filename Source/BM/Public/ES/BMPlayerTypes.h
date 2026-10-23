
#pragma once

#include "CoreMinimal.h"
#include "BMPlayerTypes.generated.h"

UENUM(BlueprintType)
enum class EBMCharacterGate : uint8
{
	None = 0,
	Walk = 1,
	Run = 2,
};


USTRUCT(BlueprintType)
struct FBMCharacterGateSetting
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxWalkSpeed = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxAcceleration = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)

	float BrakingDeceleration = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BrakingFrictionFactor = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BrakingFriction = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool UseSeparateBrakingFriction = false;
	
};
