// Fill out your copyright notice in the Description page of Project Settings.


#include "HardpointWeapon.h"
#include "Engine/World.h"
#include "Projectile.h"
#include "PlanePawn.h"

// Sets default values
UHardpointWeapon::UHardpointWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void UHardpointWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UHardpointWeapon::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	fireDelay += DeltaTime;
}
void UHardpointWeapon::Shoot_Implementation()
{
	if (fireDelay > 1 / fireRate)
	{
		//spawn projectile and assign
		FTransform SpawnTransform = GetSocketTransform("ProjectileSpawnLocation1", ERelativeTransformSpace::RTS_Component);
		SpawnTransform.SetRotation((SpawnTransform.Rotator().Add(RandomStream.FRandRange(-fireSpread, fireSpread), RandomStream.FRandRange(-fireSpread, fireSpread), 0.f)).Quaternion());
		SpawnTransform = SpawnTransform * GetComponentTransform();

		AProjectile* ProjectileInstance = GetWorld()->SpawnActor<AProjectile>(projectile, SpawnTransform.GetLocation(), SpawnTransform.Rotator());
		if (ProjectileInstance->IsValidLowLevel() && PlaneOwner) {

			if (PlaneOwner->ActorHasTag("IsFriendly")) {
				ProjectileInstance->Tags.Add(FName("IsFriendly"));
			}
			else if (PlaneOwner->ActorHasTag("IsEnemy")) {
				ProjectileInstance->Tags.Add(FName("IsEnemy"));
			}

			Cast<APawn>(PlaneOwner)->MoveIgnoreActorAdd(ProjectileInstance);

			ProjectileInstance->ProjectileMesh->SetPhysicsLinearVelocity(PlaneOwner->GetVelocity() + SpawnTransform.GetUnitAxis(EAxis::X) * fireSpeed);

			if (DamageOverride >= 0) {
				ProjectileInstance->DamageDealt = DamageOverride;
			}
			if (AreaDamageRadiusOverride >= 0) {
				ProjectileInstance->AreaDamageRadius = AreaDamageRadiusOverride;
			}

			ProjectileInstance->ProjectileMesh->SetGenerateOverlapEvents(true);
		}
		fireDelay = 0;
	}
}


