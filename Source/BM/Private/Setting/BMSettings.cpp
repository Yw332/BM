// Fill out your copyright notice in the Description page of Project Settings.


#include "Setting/BMSettings.h"

UBMSettings::UBMSettings()
{
	
}

void UBMSettings::SetSensitivity(float Value)
{
	float alpha = FMath::Clamp(Value, 0.0f, 1.0f);
	Sensitivity = FMath::Lerp(SensitivityMinClamp, SensitivityMaxClamp, alpha);
}
