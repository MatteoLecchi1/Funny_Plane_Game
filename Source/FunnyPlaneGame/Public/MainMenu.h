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

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* StartButton;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* ExitButton;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnStartButtonPressed();
	UFUNCTION()
	void OnExitButtonPressed();
};
