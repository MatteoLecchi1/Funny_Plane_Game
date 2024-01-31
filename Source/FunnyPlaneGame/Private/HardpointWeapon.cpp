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
	PlaneParent = GetParentActor();
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

	PlaneParent = GetParentActor();

	if (fireDelay > 1/fireRate)
	{
		if (GetParentActor()->IsValidLowLevelFast())
		{
			switch (ButtonToFire)
			{
			case 1:
				if (Cast<APlanePawn>(PlaneParent)->Fire1)
				{
					Shoot();
				}
				break;
			case 2:
				if (Cast<APlanePawn>(PlaneParent)->Fire2)
				{
					Shoot();
				}
				break;
			case 3:
				if (Cast<APlanePawn>(PlaneParent)->Fire3)
				{
					Shoot();
				}
				break;
			default:
				break;
			}
		}
		
		fireDelay = 0;
	}
	
}
void AHardpointWeapon::Shoot()
{
	AProjectile* ProjectileInstance = GetWorld()->SpawnActor<AProjectile>(projectile, gunMesh->GetSocketLocation("ProjectileSpawnLocation1"), GetActorRotation() + FRotator::MakeFromEuler(FVector(0, RandomStream.FRandRange(-fireSpread, fireSpread), RandomStream.FRandRange(-fireSpread, fireSpread))));
	
	if (ProjectileInstance->IsValidLowLevel()) {
		if (PlaneParent->ActorHasTag("IsFriendly")) {
			ProjectileInstance->Tags.Add(FName("IsFriendly"));
		}

		Cast<APawn>(PlaneParent)->MoveIgnoreActorAdd(ProjectileInstance);
		ProjectileInstance->ProjectileMesh->SetPhysicsLinearVelocity(ProjectileInstance->GetVelocity() + PlaneParent->GetVelocity());
		ProjectileInstance->ProjectileMesh->SetGenerateOverlapEvents(true);
	}
}


