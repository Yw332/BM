// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BMBasePlayerController.h"
#include "AbilitySystem/BMAbilitySystemComponent.h"
#include "AbilitySystem/BMAttributeSet.h"
#include "Player/Character/BMCameraCharacter.h"
#include "Player/Character/BMPlayerCharacter.h"
#include "BMPlayerController.generated.h"

enum class EGameState : uint8;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class BM_API ABMPlayerController : public ABMBasePlayerController
{
	GENERATED_BODY()


public:

	void SpawnCameraCharacter();


protected:
	
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnRep_PlayerState() override;
	virtual void SetupInputComponent() override;

	virtual void InitAbilityActorInfo();

	void Move(const FInputActionValue& Value);
	void Run_Start(const FInputActionValue& Value);
	void Run_Complete(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void Dodge(const FInputActionValue& Value);
	void InputXCombat_Start(const FInputActionValue& Value);
	void InputXCombat_Complete(const FInputActionValue& Value);
	void InputYCombat_Start(const FInputActionValue& Value);
	void InputYCombat_Complete(const FInputActionValue& Value);
	void ChangeStick(const FInputActionValue& Value);

	UFUNCTION()
	void HandleGameStateChanged(EGameState NewGameState);
	
	UFUNCTION(Server, Reliable)
	void Server_CharacterSpawn(const FTransform& SpawnTransform);

	UFUNCTION(Server, Reliable)
	void Server_CharacterSpawnCamera(const FTransform& SpawnTransform);
	
protected:
	UPROPERTY()
	TObjectPtr<UBMAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UBMAttributeSet> AttributeSet;

	
#pragma region BM Input
	UPROPERTY(EditAnywhere, Category="BM|Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, Category="BM|Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category="BM|Input")
	UInputAction* RunAction;
	
	UPROPERTY(EditAnywhere, Category="BM|Input")
	UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, Category="BM|Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category="BM|Input")
	UInputAction* DodgeAction;
	
	UPROPERTY(EditAnywhere, Category="BM|Input")
	UInputAction* InputCombatXAction;
	
	UPROPERTY(EditAnywhere, Category="BM|Input")
	UInputAction* InputCombatYAction;

	UPROPERTY(EditAnywhere, Category="BM|Input")
	UInputAction* ChangeStickAction;

	bool bRunIsPressed = false;
#pragma endregion


	// ~ Temp
	UPROPERTY(EditAnywhere, Category="BM|Temp")
	TSubclassOf<UGameplayEffect> TempEffectClass;

	// ~ End Temp
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BM")
	TSubclassOf<ABMPlayerCharacter> PlayerCHCClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BM")
	TSubclassOf<ABMCameraCharacter> CameraCharacterClass;
};
