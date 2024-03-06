// Fill out your copyright notice in the Description page of Project Settings.


#include "Hardpoint.h"
#include "HardpointWeapon.h"
#include "FunnyPlaneGameInstance.h"
#include "PlaneDefinition.h"

// Sets default values for this component's properties
UHardpoint::UHardpoint()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UHardpoint::BeginPlay()
{
	Super::BeginPlay(); 
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

void UHardpoint::AssignWeapon(const FName& WeaponKey) 
{
	if (WeaponInstance)
	{
		WeaponInstance->DestroyComponent();
	}

	auto GameInstance = UFunnyPlaneGameInstance::GetGameInstance(GetWorld());
	if (auto Definition = GameInstance->WeaponsDataTable->FindRow<FHardpointWeaponDefinition>(WeaponKey, TEXT("Weapon")))
	{
		WeaponInstance = Cast<UHardpointWeapon>(GetOwner()->AddComponentByClass(Definition->HardpointWeaponReferance, false, FTransform::Identity, false));
		WeaponInstance->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
}
