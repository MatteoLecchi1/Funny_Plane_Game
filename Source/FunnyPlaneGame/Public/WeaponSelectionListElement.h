// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlaneDefinition.h"
#include "Blueprint/UserWidget.h"
#include "WeaponSelectionListElement.generated.h"

/**
 * 
 */
UCLASS()
class FUNNYPLANEGAME_API UWeaponSelectionListElement : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = UI)
	FName Key;

	UPROPERTY(BlueprintReadWrite, Category = UI)
	FHardpointWeaponDefinition Weapon;
};
