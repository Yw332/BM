// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BMBaseCharacter.h"
#include "BMNPCCharacter.generated.h"

UCLASS()
class BM_API ABMNPCCharacter : public ABMBaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties;
	ABMNPCCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void HealthWidgetSetPercent(float value);
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	virtual void DoDamageToActor(AActor* InActor, float Damage);

	// 受击时暂停行为树，播完再恢复。
	virtual bool PlayHitReact(AActor* DamageCauser) override;
	virtual void OnHitReactFinished() override;

	/**
	 * 受击开始。C++ 在伤害生效后调用，蓝图里可以在这里暂停行为树、播蒙太奇。
	 * 蓝图里实现后，C++ 自身的蒙太奇播放会自动跳过（见 PlayHitReact）。
	 * @param DamageCauser 伤害来源，可能为空
	 */
	UFUNCTION(BlueprintImplementableEvent, Category="BM|Combat")
	void OnHitReactBegin(AActor* DamageCauser);

	/** 受击结束。蓝图里在这里恢复行为树。 */
	UFUNCTION(BlueprintImplementableEvent, Category="BM|Combat")
	void OnHitReactEnd();

	/** 由蓝图调用：告诉 C++ 受击播完了，可以解除受击状态。 */
	UFUNCTION(BlueprintCallable, Category="BM|Combat")
	void NotifyHitReactComplete();

	/** 由蓝图调用：暂停行为树（封装了一层，蓝图直接调这个就行）。 */
	UFUNCTION(BlueprintCallable, Category="BM|AI")
	void PauseBehaviorTree();

	/** 由蓝图调用：恢复行为树。 */
	UFUNCTION(BlueprintCallable, Category="BM|AI")
	void ResumeBehaviorTree();

protected:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> GameplayEffectClass;
	
};
