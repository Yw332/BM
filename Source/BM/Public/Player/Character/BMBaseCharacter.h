// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "BMBaseCharacter.generated.h"

class UAttributeSet;
class UGameplayEffect;

UCLASS()
class BM_API ABMBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABMBaseCharacter(const FObjectInitializer& ObjectInitializer);
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** 开启/关闭无敌（闪避、受击无敌帧用）。开启期间受击伤害直接丢弃。 */
	UFUNCTION(BlueprintCallable, Category="BM|Combat")
	void SetInvincible(bool bNewInvincible);

	UFUNCTION(BlueprintPure, Category="BM|Combat")
	bool IsInvincible() const { return bInvincible; }

	/** 受击后忽略后续伤害的时长（秒）。0 = 不开启受击无敌。 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BM|Combat")
	float HitInvincibleDuration = 0.3f;

	/** 命中后开一个短暂的受击无敌窗，防止同一挥砍多帧反复结算。 */
	void ApplyHitInvincibleWindow();

	/**
	 * 播放受击动画并锁住移动。
	 * 优先用 HitReactMontages（真正的蒙太奇资产，可在蓝图中配置）；
	 * 没配时才退回 HitReactSequences 动态生成。
	 * @return 是否成功开始播放
	 */
	UFUNCTION(BlueprintCallable, Category="BM|Combat")
	virtual bool PlayHitReact(AActor* DamageCauser);

	UFUNCTION(BlueprintPure, Category="BM|Combat")
	bool IsHitReacting() const { return bHitReacting; }

	/** 受击蒙太奇（首选）。在蓝图子类的细节面板里填，建议配 2~4 个随机播。 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BM|Combat")
	TArray<TObjectPtr<UAnimMontage>> HitReactMontages;

	/** 备用：受击动画序列。只是为兼容旧配置，优先用上面的 Montage。 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BM|Combat")
	TArray<TObjectPtr<UAnimSequence>> HitReactSequences;

	/** 受击蒙太奇的插槽名，要跟动画蓝图里的 Slot 节点一致 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BM|Combat")
	FName HitReactSlotName = TEXT("DefaultSlot");

	/** 受击时给的后退速度（cm/s）。0 = 不后退。 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BM|Combat")
	float HitReactKnockback = 0.f;

	/** 受击时给的向上速度（cm/s）。 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BM|Combat")
	float HitReactKnockup = 0.f;

	UPROPERTY(BlueprintReadOnly, Category="BM|Combat")
	bool bHitReacting = false;

	/** 受击动画播完后回调，子类可重写（小怪用它在此时恢复行为树） */
	UFUNCTION()
	virtual void OnHitReactFinished();

protected:
	UPROPERTY()
	TObjectPtr<UAnimMontage> ActiveHitMontage;

	FTimerHandle HitReactTimerHandle;

protected:
	UPROPERTY(BlueprintReadOnly, Category="BM|Combat")
	bool bInvincible = false;

	FTimerHandle HitInvincibleTimerHandle;

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BM|Attributes")
	TSubclassOf<UGameplayEffect> DefaultAttributes;
	
	virtual void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> EffectClass, float Level) const;
	virtual void InitDefaultAttributes();

	virtual void InitAbilityActorInfo();

};
