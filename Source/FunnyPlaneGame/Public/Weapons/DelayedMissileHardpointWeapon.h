// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HardpointWeapon.h"
#include "DelayedMissileHardpointWeapon.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FUNNYPLANEGAME_API UDelayedMissileHardpointWeapon : public UHardpointWeapon
{
	GENERATED_BODY()

public:	

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ADelayedMissileProjectile> projectile;

	UPROPERTY(EditAnywhere)
	int32 ShotAmmount = 1;
	UPROPERTY(EditAnywhere)
	float DelayBetweenShots = 0.5f;

	void Shoot(AActor* PossibleTarget) override;
	UFUNCTION()
	void ShootSingleMissile(AActor* PossibleTarget);
		
};
