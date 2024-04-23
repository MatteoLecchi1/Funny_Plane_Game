// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MissionSelectionUI.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DataTable.h"
#include "FunnyPlaneGameInstance.h"
#include "Components/ListView.h"
#include "PlaneDefinition.h"
#include "UI/MissionSelectionListEntry.h"

void UMissionSelectionUI::UpdateMissionList()
{
	auto GameInstance = UFunnyPlaneGameInstance::GetGameInstance(GetWorld());

	GameInstance->MissionsDataTable->ForeachRow<FMissionDefinition>("Plane", [&](const FName& Key, const FMissionDefinition& PlaneDefinition) {

		UMissionSelectionListEntry* Item = NewObject<UMissionSelectionListEntry>();
		Item->Mission = PlaneDefinition;
		MissionList->AddItem(Item);
	});
}