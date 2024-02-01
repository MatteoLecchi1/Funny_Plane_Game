// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlanePlayerHUD.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class FUNNYPLANEGAME_API UPlanePlayerHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void UpdateHealth(float CurrentHealth,float MaxHealth);
	void UpdateShield(float CurrentShield, float MaxShield);

	UPROPERTY(EditAnywhere,meta = (BindWidget))
	class UProgressBar* HealthBar;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* ShieldBar;

};
