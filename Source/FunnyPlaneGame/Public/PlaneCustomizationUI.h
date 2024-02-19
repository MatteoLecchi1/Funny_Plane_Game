// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlanePawn.h"
#include "PlaneCustomizationUI.generated.h"

/**
 * 
 */
UCLASS()
class FUNNYPLANEGAME_API UPlaneCustomizationUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* BeginMissionButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UListView* PlaneList;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UListView* HardpointList;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UListView* WeaponList;

	UPROPERTY(EditDefaultsOnly)
	TArray <TSubclassOf<class APlanePawn>> UnlockedPlanes;
	UPROPERTY()
	TArray<class UHardpoint*> PlaneHardpoints;
	UPROPERTY(EditDefaultsOnly)
	TArray <TSubclassOf<class UHardpointWeapon>> UnlockedWeapons;
	UPROPERTY()
	TArray<class UHardpointWeapon*> UnlockedWeaponsForSelectedHardpoint;


	virtual void NativeConstruct() override;

	UFUNCTION()
	void UpdatePlaneList();
	void OnBeginMissionButtonPressed();
};
