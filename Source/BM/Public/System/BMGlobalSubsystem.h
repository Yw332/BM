// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UI/UIPreDefine.h"
#include "BMGlobalSubsystem.generated.h"


enum class EBMMultiRoomType : uint8;
/**
 * 
 */
UCLASS()
class BM_API UBMGlobalSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()


public:

	UPROPERTY(BlueprintReadWrite)
	EBMMultiRoomType MultiRoomType = EBMMultiRoomType::LAN; 	

	UPROPERTY(BlueprintReadWrite)
	int32 PlayerCount = 2;
	
};
