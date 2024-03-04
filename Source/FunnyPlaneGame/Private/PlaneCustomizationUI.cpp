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
#include "PlaneConfigurationSaveGame.h"
#include "FunnyPlaneGameInstance.h"

void UPlaneCustomizationUI::NativeConstruct()
{
	if (BeginMissionButton)
		BeginMissionButton->OnClicked.AddDynamic(this, &UPlaneCustomizationUI::OnBeginMissionButtonPressed);
}

void UPlaneCustomizationUI::UpdatePlaneList()
{
	auto GameInstance = UFunnyPlaneGameInstance::GetGameInstance(GetWorld());

	GameInstance->PlanesDataTable->ForeachRow<FPlaneDefinition>("Plane", [&](const FName& Key, const FPlaneDefinition& PlaneDefinition) {

		UPlaneSelectionListEntry* Item = NewObject<UPlaneSelectionListEntry>();
		Item->Key = Key;
		Item->Plane = PlaneDefinition;
		PlaneList->AddItem(Item);

		if (GameInstance->SaveInstance->CurrentPlaneKey == Key)
		{
			PlaneList->SetItemSelection(Item, true);
		}
	});
}

void UPlaneCustomizationUI::UpdateHardpointList()
{
	WeaponList->ClearListItems();
	HardpointList->ClearListItems();

	auto PlaneItem = Cast<UPlaneSelectionListEntry>(PlaneList->GetSelectedItem());

	if (PlanePreviewInstance != nullptr) {
		PlanePreviewInstance->Destroy();
	}

	//save Plane 
	auto GameInstance = UFunnyPlaneGameInstance::GetGameInstance(GetWorld());
	GameInstance->SavePlaneByName(PlaneItem->Key);

	//spawn plane as preview
	PlanePreviewInstance = GetWorld()->SpawnActor<APlanePawn>(PlaneItem->Plane.PlaneReferance);

	//add all of the planes hardpoints to HUD
	if (PlaneItem != nullptr)
	{
		bool bFirst = true;
		for (auto HardpointDefinition : PlanePreviewInstance->hardpoints)
		{
			UHardpointSelectionListEntry* Item = NewObject<UHardpointSelectionListEntry>();
			Item->Hardpoint.Name = HardpointDefinition->GetAttachSocketName().ToString();

			switch (HardpointDefinition->thisHardpointTier)
			{
			case(HardpointTier::TIER0):
				Item->Hardpoint.Description = "0";
				break;
			case(HardpointTier::TIER1):
				Item->Hardpoint.Description = "00";
				break;
			case(HardpointTier::TIER2):
				Item->Hardpoint.Description = "000";
				break;
			case(HardpointTier::TIER3):
				Item->Hardpoint.Description = "0000";
				break;
			default:
				Item->Hardpoint.Description = "";
				break;
			}
			Item->Hardpoint.HardpointReferance = HardpointDefinition;
			HardpointList->AddItem(Item);

			if (bFirst)
			{
				HardpointList->SetItemSelection(Item, true);
				bFirst = false;
			}
		};
	}
}

void UPlaneCustomizationUI::UpdateWeaponList()
{
	WeaponList->ClearListItems();

	auto HardpointItem = Cast<UHardpointSelectionListEntry>(HardpointList->GetSelectedItem());
	int32 HardpointIndex = HardpointList->GetIndexForItem(HardpointItem);

	auto GameInstance = UFunnyPlaneGameInstance::GetGameInstance(GetWorld());

	auto CurrentPlane = GameInstance->GetCurrentPlane();
	FName SelectedWeapon = CurrentPlane.SavedHardpointWeapons.IsValidIndex(HardpointIndex) ? CurrentPlane.SavedHardpointWeapons[HardpointIndex] : FName();

	GameInstance->WeaponsDataTable->ForeachRow<FHardpointWeaponDefinition>("Weapon", [&](const FName& Key, const FHardpointWeaponDefinition& WeaponDefinition) {

		UWeaponSelectionListElement* Item = NewObject<UWeaponSelectionListElement>();
		Item->Key = Key;
		Item->Weapon = WeaponDefinition;
		WeaponList->AddItem(Item);

		if (Key == SelectedWeapon)
		{
			WeaponList->SetItemSelection(Item, true);
		}
	});
}

void UPlaneCustomizationUI::ChangeWeapon()
{
	auto HardpointItem = Cast<UHardpointSelectionListEntry>(HardpointList->GetSelectedItem());
	auto WeaponItem = Cast<UWeaponSelectionListElement>(WeaponList->GetSelectedItem());

	if(HardpointItem && WeaponItem)
	{
		//add weapon to preview
		HardpointItem->Hardpoint.HardpointReferance->HardpointWeapon = WeaponItem->Weapon.HardpointWeaponReferance;
		HardpointItem->Hardpoint.HardpointReferance->AssignWeapon();

		// save the weapon and its index in the array
		auto GameInstance = UFunnyPlaneGameInstance::GetGameInstance(GetWorld());
		GameInstance->SaveWeaponByNameAndHardpoint(WeaponItem->Key,HardpointList->GetIndexForItem(HardpointItem));
	}
}


void UPlaneCustomizationUI::OnBeginMissionButtonPressed()
{
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	UGameplayStatics::OpenLevel(GetWorld(), "LV_TestMap");
}