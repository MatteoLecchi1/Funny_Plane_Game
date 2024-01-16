// Fill out your copyright notice in the Description page of Project Settings.


#include "HardpointWeapon.h"
#include "Engine/World.h"
#include "Projectile.h"

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

	if (fireDelay > 1/fireRate)
	{
		AProjectile* ProjectileInstance= GetWorld()->SpawnActor<AProjectile>(projectile, GetActorLocation()+((GetActorForwardVector()*projectileSpawnLocation.X)+ (GetActorUpVector() * projectileSpawnLocation.Z)+(GetActorRightVector()*projectileSpawnLocation.Y)), GetActorRotation());
		ProjectileInstance->ProjectileMesh->ComponentVelocity += gunMesh->ComponentVelocity;
		fireDelay = 0;
	}
	
}


