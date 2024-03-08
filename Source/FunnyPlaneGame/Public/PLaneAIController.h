// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PLaneAIController.generated.h"

/**
 * 
 */
UCLASS()
class FUNNYPLANEGAME_API APLaneAIController : public AAIController
{
	GENERATED_BODY()

	UPROPERTY()
	class APlanePawn* ControlledPlane;
	UPROPERTY()
	TArray<AActor*> AllTargets;
	UPROPERTY(EditAnywhere)
	float PitchVariation = 30;
	UPROPERTY(EditAnywhere)
	float RollVariation = 30;
	UPROPERTY(EditAnywhere)
	float YawVariation = 30;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
