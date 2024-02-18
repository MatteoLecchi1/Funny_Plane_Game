// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlaneCustomizationUI.generated.h"

/**
 * 
 */
UCLASS()
class FUNNYPLANEGAME_API UPlaneCustomizationUI : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* BeginMissionButton;


	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnBeginMissionButtonPressed();
};
