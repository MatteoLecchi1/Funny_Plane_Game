// Fill out your copyright notice in the Description page of Project Settings.


#include "PlanePlayerHUD.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlanePlayerHUD::UpdateHealth(float CurrentHealth, float MaxHealth) 
{
	if (HealthBar) 
	{
		HealthBar->SetPercent(CurrentHealth/ MaxHealth);
	}
}

void UPlanePlayerHUD::UpdateShield(float CurrentShield, float MaxShield)
{
	if (ShieldBar)
	{
		ShieldBar->SetPercent(CurrentShield / MaxShield);
	}
}

void UPlanePlayerHUD::UpdateSpeed(float CurrentSpeed) 
{
	if (SpeedTextBox) 
	{
		CurrentSpeed = round(CurrentSpeed/100*3.6);
		SpeedTextBox->SetText(FText::FromString(FString::SanitizeFloat(CurrentSpeed)));
	}
}
