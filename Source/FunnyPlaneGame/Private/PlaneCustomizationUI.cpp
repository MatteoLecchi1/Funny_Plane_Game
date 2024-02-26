// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaneCustomizationUI.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/ListView.h"
#include "PlaneSelectionListElement.h"
#include "HardpointSelectionListElement.h"
#include "WeaponSelectionListElement.h"
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

void UPlaneCustomizationUI::UpdateHardpointList()
{
	HardpointList->ClearListItems();
	WeaponList->ClearListItems();

	auto PlaneItem = Cast<UPlaneSelectionListEntry>(PlaneList->GetSelectedItem());

	if (PlanePreviewInstance != nullptr) {
		PlanePreviewInstance->Destroy();
	}

	PlanePreviewInstance = GetWorld()->SpawnActor<APlanePawn>(PlaneItem->Plane.PlaneReferance);

	if (PlaneItem != nullptr)
	{
		for (auto HardpointDefinition : PlanePreviewInstance->hardpoints)
		{
			UHardpointSelectionListEntry* Item = NewObject<UHardpointSelectionListEntry>();
			Item->Hardpoint.Name = "aa";
			Item->Hardpoint.Description = "aa1";
			Item->Hardpoint.HardpointReferance = HardpointDefinition;
			HardpointList->AddItem(Item);
		};
	}
}

void UPlaneCustomizationUI::UpdateWeaponList()
{
	WeaponList->ClearListItems();

	auto HardpointItem = Cast<UHardpointSelectionListEntry>(HardpointList->GetSelectedItem());

	WeaponsDataTable->ForeachRow<FHardpointWeaponDefinition>("Weapon", [&](const FName& Key, const FHardpointWeaponDefinition& WeaponDefinition) {

		UWeaponSelectionListElement* Item = NewObject<UWeaponSelectionListElement>();
		Item->Weapon = WeaponDefinition;
		WeaponList->AddItem(Item);
	});
}


void UPlaneCustomizationUI::OnBeginMissionButtonPressed()
{
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	UGameplayStatics::OpenLevel(GetWorld(), "LV_TestMap");
}