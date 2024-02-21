// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaneCustomizationUI.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/ListView.h"
#include "PlaneSelectionListElement.h"
#include "PlanePawn.h"

void UPlaneCustomizationUI::NativeConstruct()
{
	if (BeginMissionButton)
		BeginMissionButton->OnClicked.AddDynamic(this, &UPlaneCustomizationUI::OnBeginMissionButtonPressed);
}
void UPlaneCustomizationUI::UpdatePlaneList()
{
	for (TSubclassOf<class APlanePawn> Plane : UnlockedPlanes)
	{
		UPlaneSelectionListElement* Item = NewObject<UPlaneSelectionListElement>(UPlaneSelectionListElement::StaticClass());
		Item->UpdatePlane(Cast<APlanePawn>(Plane));
		PlaneList->AddItem(Item);
	}
}
void UPlaneCustomizationUI::OnBeginMissionButtonPressed()
{
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	UGameplayStatics::OpenLevel(GetWorld(), "LV_TestMap");
}