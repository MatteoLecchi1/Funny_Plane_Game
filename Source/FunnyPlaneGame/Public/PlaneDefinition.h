#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/StaticMesh.h"
#include "PlanePawn.h"
#include "Hardpoint.h"
#include "PlaneDefinition.generated.h"

USTRUCT(BlueprintType)
struct FUNNYPLANEGAME_API FHardpointWeaponDefinition : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	FString	Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	FString	Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	HardpointTier Tier = HardpointTier::TIER0;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	bool IsUnlocked = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	bool IsEquippableByPlayer = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TSubclassOf<class UHardpointWeapon> HardpointWeaponReferance;
};

USTRUCT(BlueprintType)
struct FUNNYPLANEGAME_API FHardpointDefinition
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = Plane)
	FString	Name;

	UPROPERTY(BlueprintReadWrite, Category = Plane)
	FString	Description;

	UPROPERTY()
	class UHardpoint* HardpointReferance;
};

USTRUCT(BlueprintType)
struct FUNNYPLANEGAME_API FPlaneDefinition : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Plane)
	FString	Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Plane)
	FString	Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Plane)
	TSubclassOf<class APlanePawn> PlaneReferance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Plane)
	UTexture2D* Image;
};