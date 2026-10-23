// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Animation/BMAnimInstance.h"
#include "ES/BMAnimationTypes.h"
#include "ES/BMPlayerTypes.h"
#include "AnimDebug.generated.h"

class UBMAnimInstance;
/**
 * 
 */
namespace DebugAnim
{
	static bool bShowDebug = false;
	static bool bShowVelocity = false;
	static bool bShowTurn = false;
	static bool bShowState = false;

	/*
	static FAutoConsoleVariableRef CVarShowDebug(
		TEXT("DebugAnim.ShowDebug"),
		bShowDebug,
		TEXT("Show all debug information. Usage: DebugAnim.ShowDebug [0/1]"),
		ECVF_Default
	);

	static FAutoConsoleVariableRef CVarShowVelocity(
		TEXT("DebugAnim.ShowVelocity"),
		bShowVelocity,
		TEXT("Show the current character velocity in the top left corner of the screen. use [true/false]."),
		ECVF_Default
	);
	
	static FAutoConsoleVariableRef CVarShowTurn(
		TEXT("DebugAnim.ShowTurn"),
		bShowTurn,
		TEXT("Show the current character turn in the top left corner of the screen. use [true/false]."),
		ECVF_Default
	);

	static FAutoConsoleVariableRef CVarShowState(
		TEXT("DebugAnim.ShowState"),
		bShowState,
		TEXT("Show the current character state in the top left corner of the screen. use [true/false]."),
		ECVF_Default
	);

	static FAutoConsoleCommand ToggleAllCommand(
	  TEXT("DebugAnim.ToggleAll"),  // 这是一个命令，不需要参数
	  TEXT("Toggle all debug displays"),
	  FConsoleCommandDelegate::CreateLambda([]()
	  {
		  bShowDebug = !bShowDebug;
		  bShowVelocity = bShowDebug;
		  bShowTurn = bShowDebug;
		  bShowState = bShowDebug;
            
		  UE_LOG(LogTemp, Warning, TEXT("Debug Animation: %s"), 
			  bShowDebug ? TEXT("Enabled") : TEXT("Disabled"));
	  })
  );*/
}


UCLASS()
class UBMDebugAnimation : public UObject
{
	GENERATED_BODY()

public:
	 void DebugFunc(UBMAnimLayerInstance* AnimInstance) const ;
};

