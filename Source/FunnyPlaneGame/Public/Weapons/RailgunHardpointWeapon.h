// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HardpointWeapon.h"
#include "RailgunHardpointWeapon.generated.h"

/**
 * 
 */
UCLASS()
class FUNNYPLANEGAME_API URailgunHardpointWeapon : public UHardpointWeapon
{
	GENERATED_BODY()

public:

	void Shoot(AActor* PossibleTarget) override;

	UPROPERTY(EditAnywhere)
	float range = 50000.f;
	UPROPERTY(EditAnywhere, Category = "Collision")
	TEnumAsByte<ECollisionChannel> TraceChannelProperty = ECC_GameTraceChannel1;

	UPROPERTY(EditAnywhere, Category = "VFX")
	UNiagaraSystem* ExplosionEffect;
	UPROPERTY(EditAnywhere, Category = "VFX")
	UNiagaraSystem* TrailEffect;
};
