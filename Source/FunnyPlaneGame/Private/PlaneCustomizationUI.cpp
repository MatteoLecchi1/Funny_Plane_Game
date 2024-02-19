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
		PlaneList->AddItem(Item);
		//Item->UpdatePlaneName(FText::FromString("Baseplane"));
	}
}
void UPlaneCustomizationUI::OnBeginMissionButtonPressed()
{
	UGameplayStatics::OpenLevel(GetWorld(), "LV_TestMap");
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
}