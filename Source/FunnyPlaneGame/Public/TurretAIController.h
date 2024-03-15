// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TurretAIController.generated.h"

/**
 * 
 */
UCLASS()
class FUNNYPLANEGAME_API ATurretAIController : public AAIController
{
	GENERATED_BODY()


	UPROPERTY()
	AActor* CurrentTarget;


	UPROPERTY(EditAnywhere)
	float PitchVariation = 30;
	UPROPERTY(EditAnywhere)
	float RollVariation = 30;
	UPROPERTY(EditAnywhere)
	float YawVariation = 30;

	UPROPERTY(EditAnywhere)
	float AlignmentThreshold = 0.3;
	UPROPERTY(EditAnywhere)
	float TurretRange = 10000.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
