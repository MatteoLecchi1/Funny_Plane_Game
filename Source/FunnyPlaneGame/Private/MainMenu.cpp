// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlaneCustomizationUI.h"

void UMainMenu::NativeConstruct()
{
	if (StartButton)
		StartButton->OnClicked.AddDynamic(this, &UMainMenu::OnStartButtonPressed);
	if (ExitButton)
		ExitButton->OnClicked.AddDynamic(this, &UMainMenu::OnExitButtonPressed);
}
void UMainMenu::OnStartButtonPressed()
{
	this->RemoveFromViewport();
	if (PlaneCustomizationWidgetClass)
	{
		PlaneCustomizationWidget = CreateWidget<UPlaneCustomizationUI>(UGameplayStatics::GetPlayerController(GetWorld(), 0), PlaneCustomizationWidgetClass);
		if (PlaneCustomizationWidget) {
			PlaneCustomizationWidget->AddToViewport();
			PlaneCustomizationWidget->UpdatePlaneList();
		}
	}
}

void UMainMenu::OnExitButtonPressed()
{
	UKismetSystemLibrary::QuitGame(GetWorld(),UGameplayStatics::GetPlayerController(GetWorld(),0),EQuitPreference::Quit,true);
}
