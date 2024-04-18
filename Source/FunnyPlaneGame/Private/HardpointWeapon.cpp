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
	ReloadPercentage(100);
}

// Called every frame
void UHardpointWeapon::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	fireDelay += DeltaTime;
}
void UHardpointWeapon::Shoot(AActor* PossibleTarget)
{

}
void UHardpointWeapon::ReloadPercentage(int Percentage)
{
	CurrentAmmo += MaxAmmo * Percentage / 100;
	if (CurrentAmmo > MaxAmmo) 
	{
		CurrentAmmo = MaxAmmo;
	}
}

