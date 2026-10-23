// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BMBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "ES/BMPlayerTypes.h"
#include "GameFramework/SpringArmComponent.h"
#include "Gameplay/BMGamePreDefine.h"
#include "BMPlayerCharacter.generated.h"

class IOnlineSession;
class UBMAnimStateComponent;
class UBMAnimLayerInstance;
class UBMRootMotionComponent;

UCLASS()
class BM_API ABMPlayerCharacter : public ABMBaseCharacter
{
	GENERATED_BODY()

public:
	ABMPlayerCharacter(const FObjectInitializer& ObjectInitializer);
	
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void SetCharacterGate(EBMCharacterGate NewGate);
	EBMCharacterGate GetCharacterGate() const { return CurrentCharacterGate; }
	UFUNCTION()
	void OnRep_CharacterGate();

	TSubclassOf<UBMAnimLayerInstance> GetLinkAnimInstance() const { return LinkAnimInstance; }
	
protected:
	virtual void InitAbilityActorInfo() override;
	
	UFUNCTION(Server, Reliable)
	void Server_CharacterGate(EBMCharacterGate NewGate);

	UFUNCTION(BlueprintImplementableEvent)
	void Event_HealthChanged(float Value);
	
protected:

	/** 动画状态管理组件 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBMAnimStateComponent* AnimStateComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BM|Animation")
	TSubclassOf<UBMAnimLayerInstance> LinkAnimInstance;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CharacterGate, Category="BM|Gate")
	EBMCharacterGate CurrentCharacterGate = EBMCharacterGate::None;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BM|Gate")
	TMap<EBMCharacterGate, FBMCharacterGateSetting> CharacterGateSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArm;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* Camera;

};
