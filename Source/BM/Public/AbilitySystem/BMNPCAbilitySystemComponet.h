// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ASC/BMASCBase.h"
#include "BMNPCAbilitySystemComponet.generated.h"


UCLASS()
class BM_API UBMNPCAbilitySystemComponet : public UBMASCBase
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBMNPCAbilitySystemComponet();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
