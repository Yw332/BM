// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Pawn/BMSelectPawn.h"

#include "GameFramework/InputSettings.h"


// Sets default values
ABMSelectPawn::ABMSelectPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABMSelectPawn::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("ABMSelectPawn::BeginPlay"));
}

FTransform ABMSelectPawn::GetCameraTransform()
{
	return CameraComp->GetComponentTransform();
}

// Called every frame
void ABMSelectPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABMSelectPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

