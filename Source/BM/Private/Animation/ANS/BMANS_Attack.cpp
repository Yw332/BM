// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ANS/BMANS_Attack.h"
#include "GameFramework/Character.h"
#include "KismetTraceUtils.h"
#include "AbilitySystem/BMGameplayAbility.h"

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
		// DrawDebugLine(NowWorld, CurrentLocation, TraceLocations[Index], LineColor, false, 5.f, 0, 1.f);
		
		for (const FHitResult& Hit : OutHits)
		{
			if (!IsValid(Hit.GetActor()) || Hit.GetActor()->IsPendingKillPending()) continue;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Hit Actor: %s"), *Hit.GetActor()->GetName()));
			IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Hit.GetActor());
			if (!ASI) continue;
			TouchedActors.AddUnique(Hit.GetActor());
			Params.AddIgnoredActor(Hit.GetActor());
			
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
