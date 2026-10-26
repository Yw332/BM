// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Controller/BMPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ES/BMGameplayTags.h"
#include "ES/BMPlayerTypes.h"
#include "GameFramework/Character.h"
#include "Gameplay/GameState/BMGSMulti.h"
#include "Player/Character/BMCameraCharacter.h"
#include "Player/Character/BMPlayerCharacter.h"
#include "Player/Pawn/BMDefaultPawn.h"
#include "Player/PlayerState/BMPlayerState.h"
#include "UI/HUD/BMHUDMulti.h"

void ABMPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = false;
	
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	
	// GS 绑定
	if (ABMGSMulti* GS = GetWorld()->GetGameState<ABMGSMulti>())
	{
		GS->OnGameStateChanged.AddDynamic(this, &ThisClass::HandleGameStateChanged);
	}
}

void ABMPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	InitAbilityActorInfo();
}

void ABMPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitAbilityActorInfo();
}


void ABMPlayerController::InitAbilityActorInfo()
{
	ABMPlayerState* PS = GetPlayerState<ABMPlayerState>();
	check(PS);

	AbilitySystemComponent = Cast<UBMAbilitySystemComponent>(PS->GetAbilitySystemComponent());
	AttributeSet = Cast<UBMAttributeSet>(PS->GetAttributeSet());

	
	ABMHUD* HUD = GetHUD<ABMHUD>();
	if (HUD)
	{
		HUD->InitialOverlay(this, PS, AbilitySystemComponent, AttributeSet);
	}

}


void ABMPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(Subsystem && InputMappingContext) 
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (!IsValid(EnhancedInputComponent)) return;

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABMPlayerController::Move);
	EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &ABMPlayerController::Run_Start);
	EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &ABMPlayerController::Run_Complete);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABMPlayerController::Look);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ABMPlayerController::Jump);
	EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &ABMPlayerController::Dodge);
	EnhancedInputComponent->BindAction(InputCombatXAction, ETriggerEvent::Triggered, this, &ABMPlayerController::InputXCombat_Start);
	EnhancedInputComponent->BindAction(InputCombatXAction, ETriggerEvent::Completed, this, &ABMPlayerController::InputXCombat_Complete);
	EnhancedInputComponent->BindAction(InputCombatYAction, ETriggerEvent::Triggered, this, &ABMPlayerController::InputYCombat_Start);
	EnhancedInputComponent->BindAction(InputCombatYAction, ETriggerEvent::Completed, this, &ABMPlayerController::InputYCombat_Complete);
	EnhancedInputComponent->BindAction(ChangeStickAction, ETriggerEvent::Completed, this, &ABMPlayerController::ChangeStick);
}

void ABMPlayerController::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	ACharacter* BMCharacter = GetCharacter();
	if (!IsValid(BMCharacter)) return;

	ABMPlayerCharacter* BMPlayerCHC = Cast<ABMPlayerCharacter>(BMCharacter);
	if (IsValid(BMPlayerCHC))
	{
		if (bRunIsPressed)
		{
			BMPlayerCHC->SetCharacterGate(EBMCharacterGate::Run);
		}
		else
		{
			BMPlayerCHC->SetCharacterGate(EBMCharacterGate::Walk);
		}
	}
	/*const FRotator ControlRot = GetControlRotation();
	const FRotator YawControlRot(0.f, ControlRot.Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(YawControlRot).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawControlRot).GetUnitAxis(EAxis::Y);*/
	const FVector ForwardDirection = BMCharacter->GetActorForwardVector();
	const FVector RightDirection = BMCharacter->GetActorRightVector();
	
	BMCharacter->AddMovementInput(ForwardDirection, MovementVector.Y);
	BMCharacter->AddMovementInput(RightDirection, MovementVector.X);
}

void ABMPlayerController::Run_Start(const FInputActionValue& Value)
{
	bRunIsPressed = true;
}

void ABMPlayerController::Run_Complete(const FInputActionValue& Value)
{
	bRunIsPressed = false;
}
void ABMPlayerController::Look(const FInputActionValue& Value)
{
	FVector2D LookDirection = Value.Get<FVector2D>();
	ACharacter* BMCharacter = GetCharacter();
	if (!IsValid(BMCharacter)) return;

	BMCharacter->AddControllerYawInput(LookDirection.X);
	BMCharacter->AddControllerPitchInput(-LookDirection.Y);
}

void ABMPlayerController::Jump(const FInputActionValue& Value)
{
	ACharacter* BMCharacter = GetCharacter();
	if (!IsValid(BMCharacter)) return;
	BMCharacter->Jump();
}

void ABMPlayerController::Dodge(const FInputActionValue& Value)
{
	if (!IsValid(AbilitySystemComponent)) return;
	AbilitySystemComponent->SetPreInputTag(FBMGameplayTags::Get().Input_Dodge);
	
	FGameplayTagContainer AbilityTags = FGameplayTagContainer(FBMGameplayTags::Get().Input_Dodge);
	AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags);

	auto ApplyFunc = FTimerDelegate::CreateLambda([this]()
	{
		Cast<UBMAbilitySystemComponent>(AbilitySystemComponent)->ApplyEffectByClass(TempEffectClass);
	});

	FTimerHandle DodgeTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DodgeTimerHandle, ApplyFunc, 0.05f, false);
	
}

void ABMPlayerController::InputXCombat_Start(const FInputActionValue& Value)
{
	if (!IsValid(AbilitySystemComponent)) return;

	AbilitySystemComponent->SetPreInputTag(FBMGameplayTags::Get().Input_Combat_X);
	
	FGameplayTagContainer AbilityTags = FGameplayTagContainer(FBMGameplayTags::Get().Input_Combat_X);
	AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags);
}

void ABMPlayerController::InputXCombat_Complete(const FInputActionValue& Value)
{
	if (!IsValid(AbilitySystemComponent)) return;

	AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(FBMGameplayTags::Get().Input_Charge_Complete));
}

void ABMPlayerController::InputYCombat_Start(const FInputActionValue& Value)
{
	if (!IsValid(AbilitySystemComponent)) return;
	AbilitySystemComponent->SetPreInputTag(FBMGameplayTags::Get().Input_Combat_Y);
	
	FGameplayTagContainer AbilityTags = FGameplayTagContainer(FBMGameplayTags::Get().Input_Combat_Y);
	AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags);
}

void ABMPlayerController::InputYCombat_Complete(const FInputActionValue& Value)
{
	if (!IsValid(AbilitySystemComponent)) return;

	AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(FBMGameplayTags::Get().Input_Charge_Complete));
}

void ABMPlayerController::ChangeStick(const FInputActionValue& Value)
{
	if (!IsValid(AbilitySystemComponent)) return;

    AbilitySystemComponent->ChangeCombatType();
}

void ABMPlayerController::SpawnCameraCharacter()
{
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(GetPawn()->GetActorLocation());
	SpawnTransform.SetRotation(GetPawn()->GetActorRotation().Quaternion());
	
	Server_CharacterSpawnCamera_Implementation(SpawnTransform);
}

void ABMPlayerController::HandleGameStateChanged(EGameState NewGameState)
{
	
	if (NewGameState == EGameState::InGame)
	{
		ABMCameraCharacter* BMCharacter = Cast<ABMCameraCharacter>(GetCharacter());
		if (BMCharacter)
		{
			FTransform SpawnTransform = BMCharacter->GetCameraTransform();

			if (GetNetMode() == NM_Client)
			{
				UE_LOG(LogTemp, Error, TEXT("You are using a standard client , transform: %s"), *SpawnTransform.ToString());
			}
			
			Server_CharacterSpawn(SpawnTransform);
		}
	}
}

void ABMPlayerController::Server_CharacterSpawnCamera_Implementation(const FTransform& SpawnTransform)
{
	if (!HasAuthority()) return;
	if (GetNetMode() == NM_Client) return;
	
	if (GetPawn())
	{
		GetPawn()->Destroy();
	}

	// Spawn 正式角色
	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.Instigator = nullptr;

	if (CameraCharacterClass)
	{
		UE_LOG(LogTemp, Display, TEXT("Player CToString(ToStringToString((ToString(HC Spawn"));
	}

	UE_LOG(LogTemp, Warning, TEXT("SpawnTransform: %s in Server"), *SpawnTransform.ToString());
	
	ABMCameraCharacter* NewCharacter = GetWorld()->SpawnActor<ABMCameraCharacter>(
		CameraCharacterClass, 
		SpawnTransform.GetLocation(), 
		SpawnTransform.GetRotation().Rotator(), 
		Params
	);

	if (!IsValid(NewCharacter)) return;

	UE_LOG(LogTemp, Warning, TEXT("Server_CharacterSpawn_Implementation Called, Role=%d, Location=%s"),
	(int32)GetLocalRole(),
	*NewCharacter->GetActorLocation().ToString());

	// Possess 新角色
	Possess(NewCharacter);
}

void ABMPlayerController::Server_CharacterSpawn_Implementation(const FTransform& SpawnTransform)
{
	if (!HasAuthority()) return;
	if (GetNetMode() == NM_Client) return;
	
	if (GetPawn())
	{
		GetPawn()->Destroy();
	}

	// Spawn 正式角色
	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.Instigator = nullptr;

	if (PlayerCHCClass)
	{
		UE_LOG(LogTemp, Display, TEXT("Player CToString(ToStringToString((ToString(HC Spawn"));
	}
	

	UE_LOG(LogTemp, Warning, TEXT("SpawnTransform: %s in Server"), *SpawnTransform.ToString());
	
	ABMPlayerCharacter* NewCharacter = GetWorld()->SpawnActor<ABMPlayerCharacter>(
		PlayerCHCClass, 
		SpawnTransform.GetLocation(), 
		SpawnTransform.GetRotation().Rotator(), 
		Params
	);

	if (!IsValid(NewCharacter)) return;

	UE_LOG(LogTemp, Warning, TEXT("Server_CharacterSpawn_Implementation Called, Role=%d, Location=%s"),
	(int32)GetLocalRole(),
	*NewCharacter->GetActorLocation().ToString());

	// Possess 新角色
	Possess(NewCharacter);
}
