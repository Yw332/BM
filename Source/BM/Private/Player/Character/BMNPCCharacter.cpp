// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/BMNPCCharacter.h"

#include "AbilitySystem/BMAttributeSet.h"
#include "AbilitySystem/BMNPCAbilitySystemComponet.h"
#include "AbilitySystem/NPCAttributeSet.h"
#include "ES/BMGameplayTags.h"

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
	if (InActor)
	{
		ACharacter* Character = Cast<ACharacter>(InActor);
		if (!IsValid(Character)) return;

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
			}
		}
	}
	
}

