// Fill out your copyright notice in the Description page of Project Settings.


#include "PLaneAIController.h"
#include "PlanePawn.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlaneGameMode.h"

void APLaneAIController::BeginPlay() 
{
	Super::BeginPlay();
}
void APLaneAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetPawn()) {

		ControlledPlane = Cast<APlanePawn>(GetPawn());
		auto Component = Cast<UPrimitiveComponent>(ControlledPlane->GetRootComponent());
		Component->SetSimulatePhysics(true);
		if (ControlledPlane->ActorHasTag("IsFriendly"))
		{
			AllTargets = Cast<APlaneGameMode>(GetWorld()->GetAuthGameMode())->EnemyActors;
		}
		else if (ControlledPlane->ActorHasTag("IsEnemy"))
		{
			AllTargets = Cast<APlaneGameMode>(GetWorld()->GetAuthGameMode())->FriendlyActors;
		}

		if (AllTargets.Num()>0)
		{
			if (!CurrentTarget->IsValidLowLevelFast()) 
			{
				FRandomStream stream;
				CurrentTarget = AllTargets[stream.FRandRange(0, AllTargets.Num() - 1)];
			}
			else
			{
				//set pitch, roll and yaw
				FVector targetRelativePosition = ControlledPlane->GetActorTransform().InverseTransformPosition(CurrentTarget->GetActorLocation());
				FRotator targetRelativeRotation = targetRelativePosition.Rotation();
				FRotator targetRotation = ControlledPlane->GetActorRotation();
				ControlledPlane->CurrentRoll = FMath::GetMappedRangeValueClamped(FVector2D(-RollVariation, RollVariation), FVector2D(1., -1.), targetRelativeRotation.Yaw);
				ControlledPlane->CurrentSteer = FMath::GetMappedRangeValueClamped(FVector2D(-YawVariation, YawVariation), FVector2D(1., -1.), targetRelativeRotation.Yaw);
				ControlledPlane->CurrentPitch = FMath::GetMappedRangeValueClamped(FVector2D(-PitchVariation, PitchVariation), FVector2D(-1., 1.), targetRelativeRotation.Pitch);

			}
		}
	}
}