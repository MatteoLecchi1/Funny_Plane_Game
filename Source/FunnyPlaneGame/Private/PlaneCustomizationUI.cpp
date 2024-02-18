// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaneCustomizationUI.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UPlaneCustomizationUI::NativeConstruct()
{
	if (BeginMissionButton)
		BeginMissionButton->OnClicked.AddDynamic(this, &UPlaneCustomizationUI::OnBeginMissionButtonPressed);
}

void UPlaneCustomizationUI::OnBeginMissionButtonPressed()
{
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	UGameplayStatics::OpenLevel(GetWorld(), "LV_TestMap");
}