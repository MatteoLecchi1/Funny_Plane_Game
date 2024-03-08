// Fill out your copyright notice in the Description page of Project Settings.


#include "PLaneAIController.h"
#include "PlanePawn.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

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

		UGameplayStatics::GetAllActorsWithTag(GetWorld(), "IsFriendly", AllTargets);
		if (AllTargets.Num()>0)
		{
			auto currentTarget = AllTargets[0];

			//set pitch, roll and yaw
			FVector targetRelativePosition = ControlledPlane->GetActorTransform().InverseTransformPosition(currentTarget->GetActorLocation());
			FRotator targetRelativeRotation = targetRelativePosition.Rotation();
			FRotator targetRotation = ControlledPlane->GetActorRotation();
			ControlledPlane->CurrentRoll = FMath::GetMappedRangeValueClamped(FVector2D(-RollVariation, RollVariation), FVector2D(1., -1.), targetRelativeRotation.Yaw);
			ControlledPlane->CurrentSteer = FMath::GetMappedRangeValueClamped(FVector2D(-YawVariation, YawVariation), FVector2D(1., -1.), targetRelativeRotation.Yaw);
			ControlledPlane->CurrentPitch = FMath::GetMappedRangeValueClamped(FVector2D(-PitchVariation, PitchVariation), FVector2D(-1., 1.), targetRelativeRotation.Pitch);
		}
	}
}