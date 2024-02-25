#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/StaticMesh.h"
#include "PlaneDefinition.generated.h"

USTRUCT(BlueprintType)
struct FUNNYPLANEGAME_API FHardpointDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hardpoint)
	FString	Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hardpoint)
	FString	Description;
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
	UStaticMesh* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Plane)
	UTexture2D* Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Plane)
	TArray<FHardpointDefinition> Hardpoints;
};
