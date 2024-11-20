// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathScreen.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UDeathScreen::NativeConstruct()
{
	if (RetryButton)
		RetryButton->OnClicked.AddDynamic(this, &UDeathScreen::OnRetryButtonPressed);
	if (MainMenuButton)
		MainMenuButton->OnClicked.AddDynamic(this, &UDeathScreen::OnMainMenuButtonPressed);
}
void UDeathScreen::OnAppearOnScreen() {
	RetryButton->SetKeyboardFocus();
}
void UDeathScreen::OnRetryButtonPressed()
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), GetWorld()->GetCurrentLevel());
}

void UDeathScreen::OnMainMenuButtonPressed()
{
	UGameplayStatics::OpenLevel(GetWorld(),"LV_MainMenu");
}
