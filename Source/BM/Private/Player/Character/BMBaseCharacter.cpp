// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/BMBaseCharacter.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerState.h"
#include "TimerManager.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "ES/BMGameplayTags.h"

ABMBaseCharacter::ABMBaseCharacter(const FObjectInitializer& ObjectInitializer)  : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

UAbilitySystemComponent* ABMBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABMBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABMBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABMBaseCharacter::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> EffectClass, float Level) const
{
}

void ABMBaseCharacter::InitDefaultAttributes()
{
}

void ABMBaseCharacter::InitAbilityActorInfo()
{
	
}

void ABMBaseCharacter::SetInvincible(bool bNewInvincible)
{
	bInvincible = bNewInvincible;
}

void ABMBaseCharacter::ApplyHitInvincibleWindow()
{
	if (HitInvincibleDuration <= 0.f) return;

	bInvincible = true;

	GetWorldTimerManager().SetTimer(
		HitInvincibleTimerHandle,
		FTimerDelegate::CreateWeakLambda(this, [this]()
		{
			bInvincible = false;
		}),
		HitInvincibleDuration, false);
}

bool ABMBaseCharacter::PlayHitReact(AActor* DamageCauser)
{
	if (bHitReacting) return false;
	if (HitReactMontages.Num() == 0 && HitReactSequences.Num() == 0) return false;

	USkeletalMeshComponent* SkeletalMesh = GetMesh();
	if (!IsValid(SkeletalMesh)) return false;
	

	// 打断正在进行的攻击能力（不是暂停，是取消）。
	// 受击时还站着挥刀会非常奇怪，而且攻击 GA 会继续播蒙太奇盖住受击动画。
	// 闪避不在取消列表里 —— 闪避中应该已经是无敌状态，不会走到这里。
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		const FBMGameplayTags& BMTags = FBMGameplayTags::Get();
		FGameplayTagContainer CombatTags;
		CombatTags.AddTag(BMTags.Ability_Combat_XAll);
		CombatTags.AddTag(BMTags.Ability_Combat_Y);
		CombatTags.AddTag(BMTags.Ability_Charge_Start);
		CombatTags.AddTag(BMTags.Ability_Charge_Complete);
		ASC->CancelAbilities(&CombatTags, nullptr, nullptr);
	}

	// 不再用 DisableMovement()：那会把角色推进 MOVE_None，
	// 恢复时又硬拉回 Walking，导致角色在空中被打时穿模、按键失灵。
	// 受击只需要滑行停住，不动 movement mode。
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->StopMovementImmediately();
	}

	// 可选击退：只有在配置了非零值时执行。
	if (HitReactKnockback > 0.f || HitReactKnockup > 0.f)
	{
		FVector KnockDir = GetActorForwardVector() * -1.f;
		if (IsValid(DamageCauser))
		{
			KnockDir = GetActorLocation() - DamageCauser->GetActorLocation();
		}
		KnockDir.Z = 0.f;
		if (!KnockDir.Normalize())
		{
			KnockDir = GetActorForwardVector() * -1.f;
		}

		LaunchCharacter(KnockDir * HitReactKnockback + FVector(0.f, 0.f, HitReactKnockup), true, false);
	}
	return true;
}

void ABMBaseCharacter::OnHitReactFinished()
{
	if (!bHitReacting) return;

	bHitReacting = false;

	// 不动 MovementMode —— 它从来没被改过，保持角色原本的地面/空中状态。
	// 如果正在空中，不要强行拉回地面。
	if (IsValid(ActiveHitMontage))
	{
		if (UAnimInstance* AnimInst = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr)
		{
			if (AnimInst->Montage_IsPlaying(ActiveHitMontage))
			{
				AnimInst->Montage_Stop(0.1f, ActiveHitMontage);
			}
		}
	}

	ActiveHitMontage = nullptr;
}

