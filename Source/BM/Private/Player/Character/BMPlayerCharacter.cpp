// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/BMPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Animation/BMAnimLayerInstance.h"
#include "Player/Component/BMAnimStateComponent.h"
#include "Player/PlayerState/BMPlayerState.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/BMAbilitySystemComponent.h"
#include "AbilitySystem/BMAttributeSet.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Gameplay/GameState/BMGSMulti.h"
#include "Player/Pawn/BMSelectPawn.h"


ABMPlayerCharacter::ABMPlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;
	AnimStateComponent = CreateDefaultSubobject<UBMAnimStateComponent>(TEXT("AnimStateComponent"));

    SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 400.0f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = false;
	
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
    Camera->SetupAttachment(SpringArm);
	
}

void ABMPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// FVector SocketLocation = GetMesh()->GetSocketLocation(TEXT("Socket_Camera"));
	FVector PelvisLocation = GetMesh()->GetBoneLocation(TEXT("pelvis")) + FVector(0.0f, 0.0f, 100.0f);
	FVector RootLocation = GetActorLocation();
	FVector CameraLocation = FVector(RootLocation.X, RootLocation.Y, PelvisLocation.Z);
    SpringArm->SetWorldLocation(CameraLocation);
}

void ABMPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABMPlayerCharacter, CurrentCharacterGate);
}

void ABMPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("ABMPlayerCharacter::BeginPlay"));
	SetCharacterGate(EBMCharacterGate::Walk);
	if (IsValid(LinkAnimInstance))
	{
		GetMesh()->LinkAnimClassLayers(LinkAnimInstance);
	}
}

void ABMPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
	
}

void ABMPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	// InitAbilityActorInfo();
	
	ABMPlayerState* BMPlayerState = GetPlayerState<ABMPlayerState>();

	UAbilitySystemComponent* ASC = BMPlayerState->GetAbilitySystemComponent();
	UBMAttributeSet* AS = Cast<UBMAttributeSet>( BMPlayerState->GetAttributeSet());
	
	ASC->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			UE_LOG(LogTemp, Warning, TEXT("Broast Health Changed : %f"), Data.NewValue);
			Event_HealthChanged(Data.NewValue);
		}
	);
}

void ABMPlayerCharacter::Server_CharacterGate_Implementation(EBMCharacterGate NewGate)
{
	CurrentCharacterGate = NewGate;
	OnRep_CharacterGate();
}

void ABMPlayerCharacter::SetCharacterGate(EBMCharacterGate NewGate)
{
	if(CurrentCharacterGate == NewGate) return;
	CurrentCharacterGate = NewGate;
	
	if(!HasAuthority())
	{
		Server_CharacterGate(NewGate);
	}
	OnRep_CharacterGate();
}

void ABMPlayerCharacter::OnRep_CharacterGate()
{
	FBMCharacterGateSetting* GateSetting = CharacterGateSettings.Find(CurrentCharacterGate);
	if(GateSetting == nullptr) return;

	GetCharacterMovement()->MaxWalkSpeed = GateSetting->MaxWalkSpeed;
	GetCharacterMovement()->MaxAcceleration = GateSetting->MaxAcceleration;
	GetCharacterMovement()->BrakingDecelerationWalking = GateSetting->BrakingDeceleration;
	GetCharacterMovement()->BrakingFrictionFactor = GateSetting->BrakingFrictionFactor;
	GetCharacterMovement()->BrakingFriction = GateSetting->BrakingFriction;
	GetCharacterMovement()->bUseSeparateBrakingFriction = GateSetting->UseSeparateBrakingFriction;
}

void ABMPlayerCharacter::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();

	ABMPlayerState* BMPlayerState = GetPlayerState<ABMPlayerState>();
	check(BMPlayerState);
	BMPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(BMPlayerState, this);
	// Cast<UBMAbilitySystemComponent>(BMPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();

	AbilitySystemComponent = BMPlayerState->GetAbilitySystemComponent();
	AttributeSet = BMPlayerState->GetAttributeSet();

	UBMAttributeSet* AS = Cast<UBMAttributeSet>(AttributeSet);
	
	UE_LOG(LogTemp, Warning, TEXT("Bind on %s ASC=%s"),
		HasAuthority()?TEXT("Server"):TEXT("Client"),
		*GetNameSafe(AbilitySystemComponent));

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			UE_LOG(LogTemp, Warning, TEXT("Broast Health Changed : %f"), Data.NewValue);
			Event_HealthChanged(Data.NewValue);
		}
	);
}

