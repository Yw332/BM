// Fill out your copyright notice in the Description page of Project Settings.


#include "System/BMAssetManager.h"
#include "ES/BMGameplayTags.h"

UBMAssetManager& UBMAssetManager::Get()
{
	check(GEngine)
	UBMAssetManager* Singleton = Cast<UBMAssetManager>(GEngine->AssetManager);
	return *Singleton;
}

void UBMAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FBMGameplayTags::InitializeNativeGameplayTags();

}
