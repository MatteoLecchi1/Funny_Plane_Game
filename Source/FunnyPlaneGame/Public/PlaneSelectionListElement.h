// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlaneSelectionListElement.generated.h"

/**
 * 
 */
UCLASS()
class FUNNYPLANEGAME_API UPlaneSelectionListElement : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY()
	class APlanePawn* Plane;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* PlaneName;

	UFUNCTION()
	void UpdatePlane(APlanePawn* PlaneIn);
	UFUNCTION()
	void UpdatePlaneName(FText NewName);
};
