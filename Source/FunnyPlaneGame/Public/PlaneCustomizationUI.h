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

	UPROPERTY()
	class APlanePawn* PlanePreviewInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UMissionSelectionUI> MissionSelectionWidgetClass;
	UPROPERTY()
	class UMissionSelectionUI* MissionSelectionWidget;


	virtual void NativeConstruct() override;

	UFUNCTION()
	void UpdatePlaneList();

	UFUNCTION(BlueprintCallable)
	void UpdateHardpointList();

	UFUNCTION(BlueprintCallable)
	void UpdateWeaponList();

	UFUNCTION(BlueprintCallable)
	void ChangeWeapon();

	UFUNCTION()
	void OnBeginMissionButtonPressed();
};
