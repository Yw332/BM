// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ES/BMAnimationTypes.h"
#include "BMAnimStateComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BM_API UBMAnimStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBMAnimStateComponent();
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction) override;

	void SetControlRotation(const FRotator& NewRotation);
	FRotator GetControlRotation() const { return ControlRotation; }
	
protected:
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ControlRotation(const FRotator& NewRotation);
	
protected:
	UPROPERTY()
	ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadWrite, Replicated, Category="BM")
	FRotator ControlRotation;

private:
	friend class UBMAnimLayerInstance;

};
