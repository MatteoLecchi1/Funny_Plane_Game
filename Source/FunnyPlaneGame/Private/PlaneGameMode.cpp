// Fill out your copyright notice in the Description page of Project Settings.
#include "PlaneGameMode.h"
#include "FunnyPlaneGameInstance.h"
#include "PlaneDefinition.h"

UClass* APlaneGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	auto GameInstance = UFunnyPlaneGameInstance::GetGameInstance(GetWorld());

	auto Row = GameInstance->PlanesDataTable->FindRow<FPlaneDefinition>(GameInstance->SaveInstance->CurrentPlaneKey, TEXT("Plane"));
	if (Row && Row->PlaneReferance)
	{
		return Row->PlaneReferance;
	}

	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

APawn* APlaneGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	auto Pawn = Super::SpawnDefaultPawnAtTransform_Implementation(NewPlayer, SpawnTransform);

	if (auto PlanePawn = Cast <APlanePawn>(Pawn))
	{
		auto GameInstance = UFunnyPlaneGameInstance::GetGameInstance(GetWorld());
		if (auto CurrentPlane = GameInstance->SaveInstance->GetCurrentPlane())
		{
			PlanePawn->Configuration = *CurrentPlane;
		}
	}

	return Pawn;
}
void APlaneGameMode::AddActorToArrays(AActor* Actor) 
{
	if (Actor->ActorHasTag("IsFriendly")) 
	{
		FriendlyActors.Add(Actor);
	}
	else if (Actor->ActorHasTag("IsEnemy"))
	{
		EnemyActors.Add(Actor);
	}
}