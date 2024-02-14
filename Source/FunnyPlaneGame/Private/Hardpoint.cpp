// Fill out your copyright notice in the Description page of Project Settings.


#include "Hardpoint.h"
#include "HardpointWeapon.h"

// Sets default values for this component's properties
UHardpoint::UHardpoint()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UHardpoint::BeginPlay()
{
	Super::BeginPlay(); 
	AssignWeapon();
}


// Called every frame
void UHardpoint::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (IsShooting)
		ShootWeapon();
}

void UHardpoint::ShootWeapon()
{
	if (WeaponInstance)
	{
		WeaponInstance->Shoot();
	}
}
void UHardpoint::AssignWeapon() 
{
	if (WeaponInstance)
	{
		WeaponInstance->DestroyComponent();
	}
	if(HardpointWeapon)
	{
		WeaponInstance = Cast<UHardpointWeapon>(GetOwner()->AddComponentByClass(HardpointWeapon, false, FTransform::Identity, false));
		WeaponInstance->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
		WeaponInstance->PlaneOwner = GetOwner();
	}
}
