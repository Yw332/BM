// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Component/BMAnimStateComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UBMAnimStateComponent::UBMAnimStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UBMAnimStateComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
}

void UBMAnimStateComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBMAnimStateComponent, ControlRotation);
}

void UBMAnimStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(!OwnerCharacter) return;
	if(!OwnerCharacter->IsLocallyControlled()) return;
	
	SetControlRotation(OwnerCharacter->GetControlRotation());
	
}

void UBMAnimStateComponent::SetControlRotation(const FRotator& NewRotation)
{
	if(ControlRotation == NewRotation) return;
	ControlRotation = NewRotation;
	Server_ControlRotation(NewRotation);
}

bool UBMAnimStateComponent::Server_ControlRotation_Validate(const FRotator& NewRotation)
{
	return true;
}

void UBMAnimStateComponent::Server_ControlRotation_Implementation(const FRotator& NewRotation)
{
	ControlRotation = NewRotation;
}

