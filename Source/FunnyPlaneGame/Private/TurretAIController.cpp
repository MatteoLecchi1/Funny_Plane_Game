// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlaneGameMode.h"
#include "TurretPawn.h"

void ATurretAIController::BeginPlay()
{
	Super::BeginPlay();
}
void ATurretAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetPawn()) 
	{

		auto ControlledTurret = Cast<ATurretPawn>(GetPawn());
		auto gamemode = Cast<APlaneGameMode>(GetWorld()->GetAuthGameMode());
		TArray<AActor*> AllTargets;

		if (ControlledTurret->ActorHasTag("IsFriendly"))
		{
			AllTargets = gamemode->EnemyActors;
		}
		else if (ControlledTurret->ActorHasTag("IsEnemy"))
		{
			if (gamemode->PlayerActor)
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
			if (!CurrentTarget->IsValidLowLevelFast())
			{
				CurrentTarget = UGameplayStatics::FindNearestActor(ControlledTurret->GetActorLocation(), AllTargets, TurretRange);
			}
			else
			{
				//calculate target rotation
				FVector targetGimballRelativePosition = ControlledTurret->turretGimball->GetComponentTransform().InverseTransformPosition(CurrentTarget->GetActorLocation());

				FVector targetActorRelativePosition = ControlledTurret->GetActorTransform().InverseTransformPosition(CurrentTarget->GetActorLocation());

				FRotator targetRotation = targetGimballRelativePosition.Rotation();
				FRotator actorTargetRotation = targetActorRelativePosition.Rotation();

				auto turretgimballrotation = ControlledTurret->turretBase->GetRelativeRotation().Yaw;
				auto turretgimballrotation1 = actorTargetRotation.Yaw;

				auto YawControl = FMath::GetMappedRangeValueClamped(FVector2D(-YawVariation, YawVariation), FVector2D(-1., 1.), actorTargetRotation.Yaw - ControlledTurret->turretBase->GetRelativeRotation().Yaw);
				auto PitchControl = FMath::GetMappedRangeValueClamped(FVector2D(-PitchVariation, PitchVariation), FVector2D(-1., 1.), targetRotation.Pitch);

				ControlledTurret->CurrentYaw = YawControl;
				ControlledTurret->CurrentPitch = PitchControl;

				FVector targetRelativeDirection = targetGimballRelativePosition.GetSafeNormal();
				auto AmingFactor = FMath::Min(1., FMath::Acos(targetRelativeDirection.X));

				if (AmingFactor < AlignmentThreshold && targetGimballRelativePosition.Length() < TurretRange)
				{
					for (auto H : ControlledTurret->Hardpoints)
					{
						H->ShootWeapon(CurrentTarget);
					}
				}
			}
		}
	}
}

