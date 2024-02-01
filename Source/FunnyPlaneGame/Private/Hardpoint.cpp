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
		WeaponInstance->Destroy();
	}
	if(HardpointWeapon)
	{
		WeaponInstance = GetWorld()->SpawnActor<AHardpointWeapon>(HardpointWeapon, FVector::ZeroVector, FRotator::ZeroRotator);
		WeaponInstance->AttachToActor(this->GetOwner(), FAttachmentTransformRules::KeepRelativeTransform);
		WeaponInstance->SetActorRelativeLocation(SocketLocation);
		WeaponInstance->SetActorRelativeRotation(SocketRotation);
		WeaponInstance->PlaneOwner = this->GetOwner();
	}
}
