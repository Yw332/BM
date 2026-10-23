// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Pawn/BMDefaultPawn.h"

#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABMDefaultPawn::ABMDefaultPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;
	
}

FTransform ABMDefaultPawn::GetCameraTransform()
{
	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);

	FTransform NewTransform = FTransform();
	NewTransform.SetLocation(CameraManager->GetCameraLocation());
	NewTransform.SetRotation(CameraManager->GetCameraRotation().Quaternion());

	return NewTransform;
}

// Called when the game starts or when spawned
void ABMDefaultPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABMDefaultPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABMDefaultPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

