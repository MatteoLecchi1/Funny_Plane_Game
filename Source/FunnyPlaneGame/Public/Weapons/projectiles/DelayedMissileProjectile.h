// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "DelayedMissileProjectile.generated.h"

/**
 * 
 */
UCLASS()
class FUNNYPLANEGAME_API ADelayedMissileProjectile : public AProjectile
{
	GENERATED_BODY()
	
	public:
	UPROPERTY()
	AActor* HomingTarget;

	UPROPERTY(EditAnywhere)
	float TurnSpeed = 30;
	UPROPERTY(EditAnywhere)
	float Speed = 50000;

	UPROPERTY(EditAnywhere)
	float Homingvalue = 0;

	UPROPERTY(EditAnywhere)
	float TimeBeforeHoming = 0;

protected:
	virtual void Tick(float DeltaTime) override;
};
