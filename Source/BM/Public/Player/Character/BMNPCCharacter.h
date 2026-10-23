// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BMBaseCharacter.h"
#include "BMNPCCharacter.generated.h"

UCLASS()
class BM_API ABMNPCCharacter : public ABMBaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties;
	ABMNPCCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void HealthWidgetSetPercent(float value);
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	virtual void DoDamageToActor(AActor* InActor, float Damage);

protected:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> GameplayEffectClass;
	
};
