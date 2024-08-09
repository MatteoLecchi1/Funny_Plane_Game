// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PlaneConfigurationSaveGame.h"
#include "MissionDefinition.h"
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

	UPROPERTY(EditAnywhere)
	class UDataTable* MissionsDataTable;

	struct FMissionDefinition CurrentMission;


	UPROPERTY()
	class UPlaneConfigurationSaveGame* SaveInstance;

	UFUNCTION()
	void SavePlaneByName(FName PlaneKey);

	UFUNCTION()
	void SaveWeaponByNameAndHardpoint(FName WeaponKey, int HardpointIndex);

	FSavedPlane& GetCurrentPlane();

	static UFunnyPlaneGameInstance* GetGameInstance(const UObject* WorldContextObject);

protected:
	// Called when the game starts or when spawned
	virtual void Init() override;
};
