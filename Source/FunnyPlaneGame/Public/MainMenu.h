// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class FUNNYPLANEGAME_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UPlaneCustomizationUI> PlaneCustomizationWidgetClass;
	UPROPERTY()
	class UPlaneCustomizationUI* PlaneCustomizationWidget;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* StartButton;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* ExitButton;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnStartButtonPressed();
	UFUNCTION()
	void OnExitButtonPressed();
};
