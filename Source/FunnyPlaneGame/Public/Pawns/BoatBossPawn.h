// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pawns/BossPawn.h"
#include "BoatBossPawn.generated.h"

/**
 * 
 */
UCLASS()
class FUNNYPLANEGAME_API ABoatBossPawn : public ABossPawn
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "ascent")
	float TargetAltitude;

	UPROPERTY(EditAnywhere, Category = "ascent")
	float VerticalSpeed;
	UPROPERTY(EditAnywhere, Category = "ascent")
	float HorizontalSpeed;

	bool StartFlight;

	void SpawnNewWeapons();
	virtual void ChangePhase(int CurrentObjective) override;
	virtual void Tick(float DeltaTime) override;
};
