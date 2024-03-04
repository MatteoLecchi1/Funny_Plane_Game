// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlaneDefinition.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "PlaneSelectionListElement.generated.h"

UCLASS(BlueprintType)
class FUNNYPLANEGAME_API UPlaneSelectionListEntry : public UObject, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = UI)
	FName Key;

	UPROPERTY(BlueprintReadWrite, Category = UI)
	FPlaneDefinition Plane;
};
