// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenu::NativeConstruct()
{
	if (StartButton)
		StartButton->OnClicked.AddDynamic(this, &UMainMenu::OnStartButtonPressed);
	if (ExitButton)
		ExitButton->OnClicked.AddDynamic(this, &UMainMenu::OnExitButtonPressed);
}
void UMainMenu::OnStartButtonPressed()
{
	UGameplayStatics::OpenLevel(GetWorld(), "LV_TestMap");
}

void UMainMenu::OnExitButtonPressed()
{
	UKismetSystemLibrary::QuitGame(GetWorld(),UGameplayStatics::GetPlayerController(GetWorld(),0),EQuitPreference::Quit,true);
}
