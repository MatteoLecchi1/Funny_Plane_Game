// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/GunHardpointWeapon.h"
#include "Engine/World.h"
#include "Projectile.h"
#include "PlanePawn.h"

void UGunHardpointWeapon::Shoot(AActor* PossibleTarget)
{

	//spawn projectile and assign
	FTransform SpawnTransform = GetSocketTransform("ProjectileSpawnLocation1", ERelativeTransformSpace::RTS_Component);
	SpawnTransform.SetRotation((SpawnTransform.Rotator().Add(RandomStream.FRandRange(-fireSpread, fireSpread), RandomStream.FRandRange(-fireSpread, fireSpread), 0.f)).Quaternion());
	SpawnTransform = SpawnTransform * GetComponentTransform();

	AProjectile* ProjectileInstance = GetWorld()->SpawnActor<AProjectile>(projectile, SpawnTransform.GetLocation(), SpawnTransform.Rotator());
	if (ProjectileInstance->IsValidLowLevel()) {

		if (GetOwner()->ActorHasTag("IsFriendly")) {
			ProjectileInstance->Tags.Add(FName("IsFriendly"));
		}
		else if (GetOwner()->ActorHasTag("IsEnemy")) {
			ProjectileInstance->Tags.Add(FName("IsEnemy"));
		}

		auto PlanePawn = Cast<APawn>(GetOwner());
		PlanePawn->MoveIgnoreActorAdd(ProjectileInstance);

		ProjectileInstance->ProjectileMesh->SetPhysicsLinearVelocity(GetOwner()->GetVelocity() + SpawnTransform.GetUnitAxis(EAxis::X) * fireSpeed);

		if (DamageOverride >= 0) {
			ProjectileInstance->DamageDealt = DamageOverride;
		}
		if (AreaDamageRadiusOverride >= 0) {
			ProjectileInstance->AreaDamageRadius = AreaDamageRadiusOverride;
		}
		ProjectileInstance->ProjectileMesh->SetGenerateOverlapEvents(true);
	}
}

