// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "Components/PointLightComponent.h"
#include "MissileProjectile.generated.h"

/**
 * 
 */
UCLASS()
class FUNNYPLANEGAME_API AMissileProjectile : public AProjectile
{
	GENERATED_BODY()

public:
	UPROPERTY()
	AActor* HomingTarget;

	UPROPERTY(EditAnywhere)
	float PitchVariation = 30;
	UPROPERTY(EditAnywhere)
	float YawVariation = 30;

	UPROPERTY(EditAnywhere)
	float TurnSpeed = 30;
	UPROPERTY(EditAnywhere)
	float Speed = 50000;

	UPROPERTY(EditAnywhere)
	float Homingvalue = 0;

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
};
