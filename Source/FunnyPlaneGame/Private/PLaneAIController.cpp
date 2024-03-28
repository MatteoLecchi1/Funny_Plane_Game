// Fill out your copyright notice in the Description page of Project Settings.


#include "PLaneAIController.h"
#include "PlanePawn.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlaneGameMode.h"
#include "Hardpoint.h"

void APLaneAIController::BeginPlay() 
{
	Super::BeginPlay();
}
void APLaneAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(GetPawn())) 
	{

		ControlledPlane = Cast<APlanePawn>(GetPawn());
		auto Component = Cast<UPrimitiveComponent>(ControlledPlane->GetRootComponent());
		Component->SetSimulatePhysics(true);

		if (!IsValid(CurrentTarget)) 
		{
			RerollTarget();
		}
		else
		{
			FVector targetRelativePosition = ControlledPlane->GetActorTransform().InverseTransformPosition(CurrentTarget->GetActorLocation());

			auto AlignmentFactor = abs(FMath::Acos(FVector::DotProduct(ControlledPlane->GetActorForwardVector(), CurrentTarget->GetActorForwardVector())) / UE_HALF_PI); 

			FRotator targetRotation = targetRelativePosition.Rotation();

			auto DeltaRoll = FMath::FindDeltaAngleDegrees(ControlledPlane->GetActorRotation().Roll, CurrentTarget->GetActorRotation().Roll);
			auto YawControl = FMath::GetMappedRangeValueClamped(FVector2D(-YawVariation, YawVariation), FVector2D(-1., 1.), targetRotation.Yaw);
			auto RollControl = FMath::GetMappedRangeValueClamped(FVector2D(-RollVariation, RollVariation), FVector2D(-1., 1.), DeltaRoll);
			auto PitchControl = FMath::GetMappedRangeValueClamped(FVector2D(-PitchVariation, PitchVariation), FVector2D(-1., 1.), targetRotation.Pitch);

			ControlledPlane->CurrentSteer = FMath::Lerp(-YawControl, 0.f, AlignmentFactor);
			ControlledPlane->CurrentRoll = FMath::Lerp(RollControl, YawControl, AlignmentFactor);
			ControlledPlane->CurrentPitch = PitchControl;

			FVector targetRelativeDirection = targetRelativePosition.GetSafeNormal();
			auto AmingFactor = FMath::Min(1., FMath::Acos(targetRelativeDirection.X));

			if (AmingFactor < AlignmentThreshold)
			{
				for(auto H : ControlledPlane->Hardpoints)
				{
					H->ShootWeapon();
				}
			}
		}
	}
	else 
	{
		if (IsValid(CurrentTarget))
		{
			if (auto PossiblePalneTarget = Cast<APlanePawn>(CurrentTarget))
			{
				PossiblePalneTarget->IsAlreadyTargeted = true;
				CurrentTarget = nullptr;
			}
		}
	}
}
void APLaneAIController::RerollTarget()
{
	auto gamemode = Cast<APlaneGameMode>(GetWorld()->GetAuthGameMode());

	TArray<AActor*> AllTargets;
	AllTargets.Empty();

	if (ControlledPlane->ActorHasTag("IsFriendly"))
	{
		AllTargets = gamemode->EnemyActors;
	}
	else if (ControlledPlane->ActorHasTag("IsEnemy"))
	{
		if (!Cast<APlanePawn>(gamemode->PlayerActor)->IsAlreadyTargeted)
		{
			AllTargets.Add(gamemode->PlayerActor);
		}
		else
		{
			AllTargets = gamemode->FriendlyActors;
		}
	}
	if (AllTargets.Num() > 0)
	{
		FRandomStream stream;
		int TargetIndex = stream.FRandRange(0, AllTargets.Num() - 1);
		int i = TargetIndex;

		while (i != -1)
		{
			//sta roba non va
			auto PossibleTarget = AllTargets[i];
			if (auto PossiblePlaneTarget = Cast<APlanePawn>(PossibleTarget))
			{
				if (!PossiblePlaneTarget->IsAlreadyTargeted)
				{
					PossiblePlaneTarget->IsAlreadyTargeted = true;
					CurrentTarget = PossibleTarget;
					i = -1;
				}
				else
				{
					i++;
					if (i == AllTargets.Num())
					{
						i = 0;
					}
					if (i == TargetIndex)
					{
						CurrentTarget = PossibleTarget;
						i = -1;
					}
				}
			}
		}
	}
}