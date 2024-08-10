// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MissionSelectionUI.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DataTable.h"
#include "FunnyPlaneGameInstance.h"
#include "Components/ListView.h"
#include "PlaneDefinition.h"
#include "UI/MissionSelectionListEntry.h"
#include "MissionDefinition.h"

void UMissionSelectionUI::UpdateMissionList()
{
	auto GameInstance = UFunnyPlaneGameInstance::GetGameInstance(GetWorld());

	GameInstance->MissionsDataTable->ForeachRow<FMissionDefinition>("Mission", [&](const FName& Key, const FMissionDefinition& MissionDefinition) {
		if(MissionDefinition.IsUnlocked)
		{
			UMissionSelectionListEntry* Item = NewObject<UMissionSelectionListEntry>();
			Item->Mission = MissionDefinition;
			MissionList->AddItem(Item);
		}
	});
}
