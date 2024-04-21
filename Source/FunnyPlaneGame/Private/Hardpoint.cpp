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
	AssignWeapon(DefaultHardpointWeapon);
}

// Called every frame
void UHardpoint::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHardpoint::ShootWeapon(AActor* PossibleTarget)
{
	if (WeaponInstance)
	{
		if (WeaponInstance->CurrentAmmo > 0 && WeaponInstance->fireDelay > 1 / WeaponInstance->fireRate)
		{
			WeaponInstance->Shoot(PossibleTarget);
			WeaponInstance->CurrentAmmo--;
			WeaponInstance->fireDelay = 0;
		}
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
