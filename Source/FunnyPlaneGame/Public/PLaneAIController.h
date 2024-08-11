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
	float CollisionAvoidanceDistanceForward = 1000.f;
	UPROPERTY(EditAnywhere)
	float CollisionAvoidanceDistanceDown = 1000.f;
	UPROPERTY(EditAnywhere)
	float CollisionAvoidanceTimerMax = 3.f;
	UPROPERTY()
	float CollisionAvoidanceTimer = 0.f;
	UPROPERTY()
	TEnumAsByte<ECollisionChannel> TraceChannelProperty = ECC_Pawn;

	 void RerollTarget();
	 void AvoidCollision();
	 void TurnToTargetAndAttack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
