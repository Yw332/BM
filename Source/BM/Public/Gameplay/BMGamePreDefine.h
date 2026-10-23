// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


UENUM(BlueprintType)
enum class EGameState : uint8
{
	Preloading		= 0,
	Readying		= 1,
	Ready			= 2,
	InGame			= 3,
	GameOver		= 4,
	PlayerDead			= 5
};
