// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MissionDefinition.h"
#include "MissionSelectionUI.generated.h"

/**
 * 
 */
UCLASS()
class FUNNYPLANEGAME_API UMissionSelectionUI : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UListView* MissionList; 

	UFUNCTION()
	void UpdateMissionList();
};
