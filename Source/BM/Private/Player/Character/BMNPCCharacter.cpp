// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/BMNPCCharacter.h"

#include "AbilitySystem/BMAttributeSet.h"
#include "AbilitySystem/BMNPCAbilitySystemComponet.h"
#include "AbilitySystem/NPCAttributeSet.h"
#include "ES/BMGameplayTags.h"
#include "AIController.h"
#include "BrainComponent.h"

ABMNPCCharacter::ABMNPCCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UBMNPCAbilitySystemComponet>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UBMAttributeSet>(TEXT("AttributeSet"));

	UE_LOG(LogTemp, Warning, TEXT("NPC Ctor: ASC=%s AS=%s"),
	*GetNameSafe(AbilitySystemComponent),
	*GetNameSafe(AttributeSet));

}

// Called when the game starts or when spawned
void ABMNPCCharacter::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("%s BeginPlay: ASC=%s AS=%s"),
	*GetNameSafe(this),
	*GetNameSafe(AbilitySystemComponent),
	*GetNameSafe(AttributeSet));


	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
	else
	{
		AbilitySystemComponent = NewObject<UBMNPCAbilitySystemComponet>(this, TEXT("AbilitySystemComponentRuntime"));
		AbilitySystemComponent->RegisterComponent();
		AddOwnedComponent(AbilitySystemComponent);

		AbilitySystemComponent->SetIsReplicated(true);
		AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}

}

// Called every frame
void ABMNPCCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AttributeSet)
	{
		UBMAttributeSet* AS = Cast<UBMAttributeSet>(AttributeSet);
		HealthWidgetSetPercent(AS->GetHealth());	
	}
}

// Called to bind functionality to input
void ABMNPCCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABMNPCCharacter::DoDamageToActor(AActor* InActor, float InDamage)
{
	UE_LOG(LogTemp, Warning, TEXT("[DoDamage] %s -> %s, Damage=%.1f"),
		*GetNameSafe(this), *GetNameSafe(InActor), InDamage);

	if (!InActor) return;

	ACharacter* Character = Cast<ACharacter>(InActor);
	if (!IsValid(Character)) return;

	// 无敌帧：闪避中 / 受击后的短暂无敌窗内，伤害直接丢弃。
	// 原来这里没有任何拦截，所以无论闪避与否都一样掉血。
	if (ABMBaseCharacter* TargetChar = Cast<ABMBaseCharacter>(Character))
	{
		if (TargetChar->IsInvincible()) return;
	}

	if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Character))
	{
		if (UBMASCBase* ASC = Cast<UBMASCBase>(ASI->GetAbilitySystemComponent()))
		{
			if (!GameplayEffectClass) return;

			FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
			EffectContext.AddSourceObject(this);
			FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(GameplayEffectClass, 1, EffectContext);
			EffectSpecHandle.Data->SetSetByCallerMagnitude(FBMGameplayTags::Get().Data_Modify, -InDamage);
			
			ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

			// 命中后给目标一个短暂的受击无敌窗，防止同一挥砍多帧反复结算。
			if (ABMBaseCharacter* TargetChar = Cast<ABMBaseCharacter>(Character))
			{
				TargetChar->ApplyHitInvincibleWindow();

				// 播受击动画。由受击方自己播（而不是攻击方），
				// 这样任何来源的伤害都能触发，不用每个攻击方各写一遍。
				// 小怪在这里还会额外暂停行为树，见 ABMNPCCharacter::PlayHitReact。
				TargetChar->PlayHitReact(this);
			}
		}
	}
}

bool ABMNPCCharacter::PlayHitReact(AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("[HitReact] PlayHitReact on %s, alreadyReacting=%d"),
		*GetNameSafe(this), bHitReacting ? 1 : 0);

	if (bHitReacting) return false;

	// 把状态切到“受击中”，防止受击期间重入。
	bHitReacting = true;

	// 控制权全交给蓝图：暂停行为树、播哪个蒙太奇、播多久，
	// 全在 BP_Enemy_* 的 OnHitReactBegin 里做。
	// 蓝图接完后调 NotifyHitReactComplete() 结束受击。
	OnHitReactBegin(DamageCauser);

	return true;
}

void ABMNPCCharacter::OnHitReactFinished()
{
	Super::OnHitReactFinished();

	ResumeBehaviorTree();

	OnHitReactEnd();
}

void ABMNPCCharacter::NotifyHitReactComplete()
{
	// 给蓝图一个主动结束受击的入口（蒙太奇播完时调）。
	// 内部会走 OnHitReactFinished → 恢复行为树 → 发 OnHitReactEnd 事件。
	OnHitReactFinished();
}

void ABMNPCCharacter::PauseBehaviorTree()
{
	if (AAIController* AICon = Cast<AAIController>(GetController()))
	{
		if (UBrainComponent* Brain = AICon->GetBrainComponent())
		{
			Brain->PauseLogic(TEXT("HitReact"));
		}
	}
}

void ABMNPCCharacter::ResumeBehaviorTree()
{
	if (AAIController* AICon = Cast<AAIController>(GetController()))
	{
		if (UBrainComponent* Brain = AICon->GetBrainComponent())
		{
			Brain->ResumeLogic(TEXT("HitReact"));
		}
	}
}

