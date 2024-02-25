// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlaneDefinition.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "HardpointSelectionListElement.generated.h"

UCLASS(BlueprintType)
class FUNNYPLANEGAME_API UHardpointSelectionListEntry : public UObject, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = UI)
	FHardpointDefinition Hardpoint;
};
