// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ANS/BMANS_Attack.h"
#include "GameFramework/Character.h"
#include "KismetTraceUtils.h"
#include "AbilitySystem/BMGameplayAbility.h"
#include "Player/Character/BMBaseCharacter.h"

void UBMANS_Attack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	if (!MeshComp || !MeshComp->GetOwner()) return;

	if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(MeshComp->GetOwner()))
	{
		if (UBMAbilitySystemComponent* ASC = Cast<UBMAbilitySystemComponent>(ASI->GetAbilitySystemComponent()))
		{
			if (UBMGameplayAbility* GA = Cast<UBMGameplayAbility>(ASC->GetAnimatingAbility()))
			{
				InfiniteEffectClass = GA->GetInfiniteEffectClass();
				InstantEffectClass = GA->GetInstantEffectClass();
				HitDamage =  GA->GetHitDamage();
				if (!InfiniteEffectClass || !InstantEffectClass)
				{
					UE_LOG(LogTemp, Error, TEXT("Ability's Effect Class is null, please check it ! %s "), *ASC->GetAnimatingAbility()->GetName());
					return;
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("GA is null, %s"), *this->GetName());
			}
			
		}
	}

	if (!InfiniteEffectClass || !InstantEffectClass)
	{
		return;
	}
	
	TraceLocations.Empty();
	Params.AddIgnoredActor(MeshComp->GetOwner());
	
	TraceLocations.Empty();
	for (FName SocketName : TraceSocketNames)
	{
		TraceLocations.Add(MeshComp->GetSocketLocation(SocketName));
	}
}

void UBMANS_Attack::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (!InfiniteEffectClass || !InstantEffectClass)
	{
		return;
	}
	
	if (TraceLocations.IsEmpty()) return;
	
	for (int32 Index = 0 ; Index < TraceSocketNames.Num(); ++Index)
	{
		FName SocketName = TraceSocketNames[Index];
		FVector CurrentLocation = MeshComp->GetSocketLocation(SocketName);

		TArray<FHitResult> OutHits;
		UWorld* NowWorld = MeshComp->GetWorld();
		if(!NowWorld) return;
		
		NowWorld->LineTraceMultiByChannel(OutHits, CurrentLocation, TraceLocations[Index], ECC_Pawn, Params);
		FColor LineColor = OutHits.Num() > 0 ? FColor::Red : FColor::Green;
		DrawDebugLine(NowWorld, CurrentLocation, TraceLocations[Index], LineColor, false, 5.f, 0, 1.f);
		
		for (const FHitResult& Hit : OutHits)
		{
			if (!IsValid(Hit.GetActor()) || Hit.GetActor()->IsPendingKillPending()) continue;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Hit Actor: %s"), *Hit.GetActor()->GetName()));
			IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Hit.GetActor());
			if (!ASI) continue;
			TouchedActors.AddUnique(Hit.GetActor());
			Params.AddIgnoredActor(Hit.GetActor());

			// 击退：只对 Character 生效（对静态物件调 LaunchCharacter 没有意义）。
			// 方向默认沿攻击方朝向（从攻击者指向目标），可选改成“背离攻击者”。
			if (KnockbackStrength > 0.f)
			{
				if (ACharacter* HitCharacter = Cast<ACharacter>(Hit.GetActor()))
				{
					FVector KnockDir = HitCharacter->GetActorLocation() - MeshComp->GetOwner()->GetActorLocation();
					KnockDir.Z = 0.f;
					if (!KnockDir.Normalize())
					{
						// 两者位置重合时退化成攻击者正前方，避免零向量。
						KnockDir = MeshComp->GetOwner()->GetActorForwardVector();
						KnockDir.Z = 0.f;
						KnockDir.Normalize();
					}

					const FVector Launch = KnockDir * KnockbackStrength + FVector(0.f, 0.f, KnockbackUpward);
					// bXYOverride = true：覆盖目标当前水平速度，否则减速中的目标只会被轻微推一下。
					HitCharacter->LaunchCharacter(Launch, true, false);
				}
			}
			
			if (UBMASCBase* ASC = Cast<UBMASCBase>(ASI->GetAbilitySystemComponent()))
			{
				if (InstantEffectClass)
				{
					FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
					EffectContext.AddSourceObject(this);
					FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(InstantEffectClass, 1, EffectContext);
					EffectSpecHandle.Data->SetSetByCallerMagnitude(FBMGameplayTags::Get().Data_Modify, -HitDamage);
					ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
				}

				if (InfiniteEffectClass)
				{
					ASC->ApplyEffectByClass(InfiniteEffectClass);
				}

				// 触发受击。玩家打小怪走的是这条路径（不是 DoDamageToActor），
				// 所以受击必须在这里也触发一次，否则小怪永远不播受击。
				// 注意：只对同一挥砍里第一次命中的目标触发，避免每帧重复打断。
				if (ABMBaseCharacter* TargetChar = Cast<ABMBaseCharacter>(Hit.GetActor()))
				{
					// 受击无敌窗兼做去重：已经在无敌窗内就不再触发受击。
					if (!TargetChar->IsInvincible())
					{
						TargetChar->ApplyHitInvincibleWindow();
						TargetChar->PlayHitReact(MeshComp->GetOwner());
					}
				}
			}
			
		}
		
		TraceLocations[Index] = CurrentLocation;
	}
}

void UBMANS_Attack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (!MeshComp || !MeshComp->GetOwner()) return;
	if (!InfiniteEffectClass || !InstantEffectClass)
	{
		return;
	}

	for (TWeakObjectPtr<AActor> SingleActor : TouchedActors)
	{
		if (SingleActor.IsValid())
		{
			ACharacter* TouchedCharacter = Cast<ACharacter>(SingleActor.Get());
			if (!IsValid(TouchedCharacter)) return;
			
			if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(TouchedCharacter))
			{
				if (UBMAbilitySystemComponent* ASC = Cast<UBMAbilitySystemComponent>(ASI->GetAbilitySystemComponent()))
				{
					ASC->RemoveEffectByClass(InfiniteEffectClass);
				}
			}
		}
	}
	
	Params = FCollisionQueryParams();
	TraceLocations.Empty();
}
