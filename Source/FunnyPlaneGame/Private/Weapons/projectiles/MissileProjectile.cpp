// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/projectiles/MissileProjectile.h"
#include "Kismet/KismetMathLibrary.h"

void AMissileProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 

	if(HomingTarget)
	{
		FVector targetRelativePosition = this->GetActorTransform().InverseTransformPosition(HomingTarget->GetActorLocation());
		targetRelativePosition.Normalize();
		//check if the missile surpassed or was missfired
		if (targetRelativePosition.X <= Homingvalue)
		{
			HomingTarget = nullptr;
		}
		else
		{
			//turn to the target
			FRotator targetRotation = targetRelativePosition.Rotation();

			auto YawControl = FMath::Clamp(targetRotation.Yaw, -1., 1.);
			auto PitchControl = FMath::Clamp(targetRotation.Pitch ,-1., 1.);

			FRotator DeltaRotation = FRotator(PitchControl, YawControl, 0.f);

			this->AddActorLocalRotation(DeltaRotation * TurnSpeed * DeltaTime);
		}
	}
	this->ProjectileMesh->SetPhysicsLinearVelocity(Speed* this->GetActorForwardVector());
}