// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaneCustomizationUI.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/ListView.h"
#include "PlaneSelectionListElement.h"
#include "HardpointSelectionListElement.h"
#include "Engine/DataTable.h"

void UPlaneCustomizationUI::NativeConstruct()
{
	if (BeginMissionButton)
		BeginMissionButton->OnClicked.AddDynamic(this, &UPlaneCustomizationUI::OnBeginMissionButtonPressed);
}

void UPlaneCustomizationUI::UpdatePlaneList()
{
	PlanesDataTable->ForeachRow<FPlaneDefinition>("Plane", [&](const FName& Key, const FPlaneDefinition& PlaneDefinition) {

		UPlaneSelectionListEntry* Item = NewObject<UPlaneSelectionListEntry>();
		Item->Plane = PlaneDefinition;
		PlaneList->AddItem(Item);
	});
}

void UPlaneCustomizationUI::UpdateWeaponList()
{
	// TODO
}

void UPlaneCustomizationUI::UpdateHardpointList()
{
	HardpointList->ClearListItems();

	auto PlaneItem = Cast<UPlaneSelectionListEntry>(PlaneList->GetSelectedItem());

	if (PlaneItem != nullptr)
	{
		for (auto& HardpointDefinition : PlaneItem->Plane.Hardpoints)
		{
			UHardpointSelectionListEntry* Item = NewObject<UHardpointSelectionListEntry>();
			Item->Hardpoint = HardpointDefinition;
			HardpointList->AddItem(Item);
		};
	}
}

void UPlaneCustomizationUI::OnBeginMissionButtonPressed()
{
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	UGameplayStatics::OpenLevel(GetWorld(), "LV_TestMap");
}