// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DeathScreen.generated.h"

/**
 * 
 */
UCLASS()
class FUNNYPLANEGAME_API UDeathScreen : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* RetryButton;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* MainMenuButton;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnRetryButtonPressed();
	UFUNCTION()
	void OnMainMenuButtonPressed();

};
