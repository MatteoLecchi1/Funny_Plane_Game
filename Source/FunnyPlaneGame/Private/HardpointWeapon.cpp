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
		
		AProjectile* ProjectileInstance = GetWorld()->SpawnActor<AProjectile>(projectile,gunMesh->GetSocketLocation("ProjectileSpawnLocation1"), GetActorRotation()+FRotator::MakeFromEuler(FVector((0, FMath::FRandRange(-fireSpread,fireSpread), FMath::FRandRange(-fireSpread, fireSpread)))));
		Cast<APawn>(GetParentActor())->MoveIgnoreActorAdd(ProjectileInstance);
		if (GetParentActor()->IsValidLowLevelFast()) 
			ProjectileInstance->ProjectileMesh->ComponentVelocity += GetParentActor()->GetVelocity();
		fireDelay = 0;
	}
	
}


