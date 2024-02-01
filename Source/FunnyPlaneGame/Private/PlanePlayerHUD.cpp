// Fill out your copyright notice in the Description page of Project Settings.


#include "PlanePlayerHUD.h"
#include "Components/ProgressBar.h"

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
