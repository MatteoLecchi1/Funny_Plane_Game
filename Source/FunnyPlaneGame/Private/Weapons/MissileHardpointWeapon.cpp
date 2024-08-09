// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/MissileHardpointWeapon.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Projectile.h"
#include "Weapons/projectiles/MissileProjectile.h"
#include "PlanePawn.h"

void UMissileHardpointWeapon::Shoot(AActor* PossibleTarget)
{
	Super::Shoot(PossibleTarget);

	for(int i = 0; i < ShotAmmount; i++)
	{
		FTimerHandle TimerHandle;
		FTimerDelegate ShootDelegate;
		ShootDelegate.BindUFunction(this, FName("ShootSingleMissile"), PossibleTarget);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, ShootDelegate, DelayBetweenShots*i + 0.01f, false);
	}
}
void UMissileHardpointWeapon::ShootSingleMissile(AActor* PossibleTarget) 
{

	//spawn projectile and assign
	FTransform SpawnTransform = GetSocketTransform("ProjectileSpawnLocation1", ERelativeTransformSpace::RTS_Component);
	SpawnTransform.SetRotation((SpawnTransform.Rotator().Add(RandomStream.FRandRange(-fireSpread, fireSpread), RandomStream.FRandRange(-fireSpread, fireSpread), 0.f)).Quaternion());
	SpawnTransform = SpawnTransform * GetComponentTransform();

	AMissileProjectile* ProjectileInstance = GetWorld()->SpawnActor<AMissileProjectile>(projectile, SpawnTransform.GetLocation(), SpawnTransform.Rotator());
	if (ProjectileInstance->IsValidLowLevel())
	{
		if (GetOwner()->ActorHasTag("IsFriendly")) {
			ProjectileInstance->Tags.Add(FName("IsFriendlyProjectile"));
		}
		else if (GetOwner()->ActorHasTag("IsEnemy")) {
			ProjectileInstance->Tags.Add(FName("IsEnemyProjectile"));
		}

		auto PlanePawn = Cast<APawn>(GetOwner());
		PlanePawn->MoveIgnoreActorAdd(ProjectileInstance);

		if (DamageOverride >= 0) {
			ProjectileInstance->DamageDealt = DamageOverride;
		}
		if (AreaDamageRadiusOverride >= 0) {
			ProjectileInstance->AreaDamageRadius = AreaDamageRadiusOverride;
		}
		ProjectileInstance->ProjectileMesh->SetGenerateOverlapEvents(true);

		//homing system
		if (IsValid(PossibleTarget))
		{
			ProjectileInstance->HomingTarget = PossibleTarget;
		}
	}

	SpawnBarrelEffect(SpawnTransform);
}