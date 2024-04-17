// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "MissileProjectile.generated.h"

/**
 * 
 */
UCLASS()
class FUNNYPLANEGAME_API AMissileProjectile : public AProjectile
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
};
