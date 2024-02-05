// Fill out your copyright notice in the Description page of Project Settings.


#include "HardpointWeapon.h"
#include "Engine/World.h"
#include "Projectile.h"
#include "PlanePawn.h"

// Sets default values
AHardpointWeapon::AHardpointWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	gunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun Mesh"));
}

// Called when the game starts or when spawned
void AHardpointWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHardpointWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	fireDelay += DeltaTime;
}
void AHardpointWeapon::Shoot_Implementation()
{
	if (fireDelay > 1 / fireRate)
	{
		//spawn projectile and assign
		AProjectile* ProjectileInstance = GetWorld()->SpawnActor<AProjectile>(projectile, gunMesh->GetSocketLocation("ProjectileSpawnLocation1"), gunMesh->GetSocketRotation("ProjectileSpawnLocation1") + FRotator::MakeFromEuler(FVector(0, RandomStream.FRandRange(-fireSpread, fireSpread), RandomStream.FRandRange(-fireSpread, fireSpread))));
		if (ProjectileInstance->IsValidLowLevel() && PlaneOwner) {

			if (PlaneOwner->ActorHasTag("IsFriendly")) {
				ProjectileInstance->Tags.Add(FName("IsFriendly"));
			}
			else if (PlaneOwner->ActorHasTag("IsEnemy")) {
				ProjectileInstance->Tags.Add(FName("IsEnemy"));
			}


			Cast<APawn>(PlaneOwner)->MoveIgnoreActorAdd(ProjectileInstance);
			ProjectileInstance->ProjectileMesh->SetPhysicsLinearVelocity(ProjectileInstance->GetVelocity() + PlaneOwner->GetVelocity());

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


