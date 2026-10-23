// Fill out your copyright notice in the Description page of Project Settings.


#include "ES/BMGameplayTags.h"

#include "GameplayTagsManager.h"

FBMGameplayTags FBMGameplayTags::GameplayTags;

void FBMGameplayTags::InitializeNativeGameplayTags()
{
	/* Input Tags */
	GameplayTags.Input_Combat_X = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Combat.X"));
	GameplayTags.Input_Combat_Y = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Combat.Y"));
	GameplayTags.Input_Dodge = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Dodge"));
	GameplayTags.Input_Charge_Start = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Charge.Start"));
	GameplayTags.Input_Charge_Complete = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Input.Charge.Complete"));

	/* State Tags */
	GameplayTags.State_StickType_Poke = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.StickType.Poke"));
	GameplayTags.State_StickType_Split = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.StickType.Split"));
	GameplayTags.State_StickType_Stand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.StickType.Stand"));

	GameplayTags.State_StickLevel_One = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.StickLevel.One"));
	GameplayTags.State_StickLevel_Two = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.StickLevel.Two"));
	GameplayTags.State_StickLevel_Three = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.StickLevel.Three"));
	GameplayTags.State_StickLevel_Four = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("State.StickLevel.Four"));

	/* Ability Tags */
	GameplayTags.Ability_Dodge_Normal = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Dodge.Normal"));
	GameplayTags.Ability_Dodge_Perfect = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Dodge.Perfect"));

	GameplayTags.Ability_CombatWindow = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.CombatWindow"));
	GameplayTags.Ability_PreInputWindow = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.PreInputWindow"));

	GameplayTags.Ability_Combat_X = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Combat.X"));
	GameplayTags.Ability_Combat_XX = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Combat.XX"));
	GameplayTags.Ability_Combat_XXX = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Combat.XXX"));
	GameplayTags.Ability_Combat_XXXX = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Combat.XXXX"));
	GameplayTags.Ability_Combat_XXXXX = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Combat.XXXXX"));
	GameplayTags.Ability_Combat_XAll = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Combat.XAll"));

	GameplayTags.Ability_Combat_XsY = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Combat.XsY"));
	GameplayTags.Ability_Combat_XsYX = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Combat.XsYX"));
	GameplayTags.Ability_Combat_XsYY = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Combat.XsYY"));

	GameplayTags.Ability_Combat_Y = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Combat.Y"));

	GameplayTags.Ability_Charge_Start =  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Charge.Start"));
	GameplayTags.Ability_Charge_Complete = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Ability.Charge.Complete"));
	
	/* Gameplay Event Tags */
    GameplayTags.GameplayEvent_Hit = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayEvent.Hit"));
	GameplayTags.GameplayEvent_Animation_ChargeStartComplete = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayEvent.Animation.ChargeStartComplete"));
	GameplayTags.GameplayEvent_Animation_ChargeOver = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayEvent.Animation.ChargeOver"));

	/* Data Tags */
	GameplayTags.Data_Modify = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Data.Modify"));
}
