// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HardpointWeapon.h"
#include "MissileHardpointWeapon.generated.h"

/**
 * 
 */
UCLASS()
class FUNNYPLANEGAME_API UMissileHardpointWeapon : public UHardpointWeapon
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> projectile;

	void Shoot() override;
};
