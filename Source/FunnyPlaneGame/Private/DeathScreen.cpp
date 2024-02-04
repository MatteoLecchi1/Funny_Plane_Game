// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathScreen.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UDeathScreen::NativeConstruct()
{
	if (RetryButton)
		RetryButton->OnClicked.AddDynamic(this, &UDeathScreen::OnRetryButtonPressed);
	if (RetryButton)
		MainMenuButton->OnClicked.AddDynamic(this, &UDeathScreen::OnMainMenuButtonPressed);
}

void UDeathScreen::OnRetryButtonPressed()
{
	UGameplayStatics::OpenLevel(GetWorld(), GetWorld()->GetCurrentLevel()->GetFName());
}

void UDeathScreen::OnMainMenuButtonPressed()
{

}
