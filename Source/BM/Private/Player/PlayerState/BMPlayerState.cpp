// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerState/BMPlayerState.h"

#include "AbilitySystem/BMAbilitySystemComponent.h"
#include "AbilitySystem/BMAttributeSet.h"
#include "Gameplay/Mode/BMGMMulti.h"
#include "Net/UnrealNetwork.h"
#include "Player/Controller/BMPlayerController.h"

ABMPlayerState::ABMPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UBMAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	AttributeSet = CreateDefaultSubobject<UBMAttributeSet>(TEXT("AttributeSet"));

	NetUpdateFrequency = 100.0f;
}

void ABMPlayerState::BeginPlay()
{
	Super::BeginPlay();

	UBMAttributeSet* AS = Cast<UBMAttributeSet>(AttributeSet);
	if (AS)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				if (IsDead())
				{
					OnPlayerStateDeath.Broadcast();
				    ABMPlayerController* PC = Cast<ABMPlayerController>(GetPlayerController());

					if (PC)
					{
						PC->SpawnCameraCharacter();
					}

					ABMGMMulti* GM = Cast<ABMGMMulti>(GetWorld()->GetAuthGameMode());
					if (GM)
					{
						GM->OnPlayerDied();
					}
					
				}
			});
	}
}

void ABMPlayerState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}


UAbilitySystemComponent* ABMPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABMPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ABMPlayerState, AttributePoints, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ABMPlayerState, bIsDead, COND_None, REPNOTIFY_Always);
}

UAttributeSet* ABMPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

void ABMPlayerState::AddAttributePoints(int32 InPoints)
{
	AttributePoints += InPoints;
	OnAttributePointsChanged.Broadcast(AttributePoints);
}

bool ABMPlayerState::IsDead()
{
	if (bIsDead) return true;
	
	UBMAttributeSet* AS = Cast<UBMAttributeSet>(AttributeSet);
	if (AS->GetHealth() <= 0.000001f)
	{
		bIsDead = true;
		return true;
	}
	return false;
}

void ABMPlayerState::OnRep_AttributePoints(int32 OldAttributePoints)
{
	OnAttributePointsChanged.Broadcast(AttributePoints);
}

void ABMPlayerState::OnRep_Dead()
{
	OnPlayerStateDeath.Broadcast();
}

