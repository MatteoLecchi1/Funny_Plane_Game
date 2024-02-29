// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PlaneConfigurationSaveGame.h"
#include "FunnyPlaneGameInstance.generated.h"

UCLASS()
class FUNNYPLANEGAME_API UFunnyPlaneGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	class UDataTable* PlanesDataTable;

	UPROPERTY(EditAnywhere)
	class UDataTable* WeaponsDataTable;

	UPROPERTY()
	class UPlaneConfigurationSaveGame* SaveInstance;

	UFUNCTION()
	void SavePlaneByName(FString PlaneName);
	UFUNCTION()
	void SaveWeaponByNameAndHardpoint(FString WeaponName, int HardpointIndex);

	static UFunnyPlaneGameInstance* GetGameInstance(const UObject* WorldContextObject);

protected:
	// Called when the game starts or when spawned
	virtual void Init() override;
};
