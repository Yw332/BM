// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

struct FBMGameplayTags
{
public:
	static const FBMGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();

	// ~ Input Tags
	FGameplayTag Input_Combat_X;
	FGameplayTag Input_Combat_Y;
	FGameplayTag Input_Dodge;
	FGameplayTag Input_Charge_Start;
	FGameplayTag Input_Charge_Complete;
	// ~ End Input Tags

	//  ~ State Tags
	FGameplayTag State_StickType_Stand;
	FGameplayTag State_StickType_Split;
	FGameplayTag State_StickType_Poke;
	
	FGameplayTag State_StickLevel_One;
	FGameplayTag State_StickLevel_Two;
	FGameplayTag State_StickLevel_Three;
	FGameplayTag State_StickLevel_Four;
	

	// ~ Ability Tags
	FGameplayTag Ability_Dodge_Normal;
	FGameplayTag Ability_Dodge_Perfect;

	FGameplayTag Ability_CombatWindow;
	FGameplayTag Ability_PreInputWindow;
	
	FGameplayTag Ability_Combat_X;
	FGameplayTag Ability_Combat_XX;
	FGameplayTag Ability_Combat_XXX;
	FGameplayTag Ability_Combat_XXXX;
	FGameplayTag Ability_Combat_XXXXX;
	FGameplayTag Ability_Combat_XAll;

	FGameplayTag Ability_Combat_XsY;
	FGameplayTag Ability_Combat_XsYX;
	FGameplayTag Ability_Combat_XsYY;

	FGameplayTag Ability_Combat_Y;

	FGameplayTag Ability_Charge_Start;
	FGameplayTag Ability_Charge_Complete;
	// ~ End Ability Tags


	// ~ Gameplay Event Tags
	FGameplayTag GameplayEvent_Hit;
	FGameplayTag GameplayEvent_Animation_ChargeStartComplete;
	FGameplayTag GameplayEvent_Animation_ChargeOver;
	// ~ End Gameplay Event Tags


	// ~ Data
	FGameplayTag Data_Modify;
	// ~ End Data

private:
	static FBMGameplayTags GameplayTags;
};
