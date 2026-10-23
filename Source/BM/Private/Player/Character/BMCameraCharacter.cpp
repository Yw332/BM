// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/BMCameraCharacter.h"

#include "GameFramework/InputSettings.h"


// Sets default values
ABMCameraCharacter::ABMCameraCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(RootComponent);
}

FTransform ABMCameraCharacter::GetCameraTransform()
{
	if (!CameraComp) return FTransform();
	return CameraComp->GetComponentTransform();
}

// Called when the game starts or when spawned
void ABMCameraCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABMCameraCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABMCameraCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

