// Fill out your copyright notice in the Description page of Project Settings.
#include "PlaneGameMode.h"
#include "FunnyPlaneGameInstance.h"
#include "PlaneDefinition.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MissionDefinition.h"
#include "LevelObjects/Spawner.h"

UClass* APlaneGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	auto GameInstance = UFunnyPlaneGameInstance::GetGameInstance(GetWorld());

	auto Row = GameInstance->PlanesDataTable->FindRow<FPlaneDefinition>(GameInstance->SaveInstance->PlaneConfig.CurrentPlaneKey, TEXT("Plane"));
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
		if (auto CurrentPlane = GameInstance->SaveInstance->PlaneConfig.GetCurrentPlane())
		{
			PlanePawn->Configuration = *CurrentPlane;
		}
	}

	return Pawn;
}

void APlaneGameMode::BeginPlay()
{
	Super::BeginPlay();
}
void APlaneGameMode::Tick(float DeltaTime)
{
	if (Timer == 1) {
		Timer = -1;
		auto GameInstance = UFunnyPlaneGameInstance::GetGameInstance(GetWorld());
		if (GameInstance)
		{
			MissionDefinition = GameInstance->CurrentMission;
			CurrentObjective = -1;
			JumpToNextObjective();
		}
	}
	else if (Timer == 0)
	{
		Timer = 1;
	}
}
void APlaneGameMode::AddActorToArrays(AActor* Actor) 
{
	if (Actor->ActorHasTag("IsFriendly")) 
	{
		FriendlyActors.Add(Actor);

		auto MarkerWidget = Cast<UWidgetComponent>(Actor->GetComponentsByTag(UWidgetComponent::StaticClass(), "MarkerWidget")[0]);
		if (FriendlyMarkerWidget)
		{
			UUserWidget* MarkerWidgetinstance = CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), FriendlyMarkerWidget);
			MarkerWidget->SetWidget(MarkerWidgetinstance);
			MarkerWidget->SetWidgetSpace(EWidgetSpace::Screen);
		}
	}
	else if (Actor->ActorHasTag("IsEnemy"))
	{
		EnemyActors.Add(Actor);

		auto MarkerWidget = Cast<UWidgetComponent>(Actor->GetComponentsByTag(UWidgetComponent::StaticClass(), "MarkerWidget")[0]);
		if (EnemyMarkerWidget)
		{
			UUserWidget* MarkerWidgetinstance = CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), EnemyMarkerWidget);
			MarkerWidget->SetWidget(MarkerWidgetinstance);
			MarkerWidget->SetWidgetSpace(EWidgetSpace::Screen);
		}
	}
}
void APlaneGameMode::RemoveActorFromArrays(AActor* Actor)
{
	if (Actor->ActorHasTag("IsFriendly"))
	{
		FriendlyActors.Remove(Actor);
	}
	else if (Actor->ActorHasTag("IsEnemy"))
	{
		EnemyActors.Remove(Actor);
		UpdateTargets(Actor);
	}
}
void APlaneGameMode::JumpToNextObjective() 
{
	CurrentObjective++;
	if (CurrentObjective == MissionDefinition.Objectives.Num()) 
	{
		auto GameInstance = UFunnyPlaneGameInstance::GetGameInstance(GetWorld());
		for (FName missionToUnlock : MissionDefinition.MissionsToUnlock) {
			GameInstance->UnlockMission(missionToUnlock);
		}

		FTimerHandle TimerHandle;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, FName("ExitMission"));
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, Delegate, 5.f, false);
	}
	else
	{

		SpawnActorsFromSpawnerTag(MissionDefinition.Objectives[CurrentObjective].SpawnerTags);

		AssignTargets();
	}
}
void APlaneGameMode::AssignTargets()
{
	FObjectiveDefinition Objective = MissionDefinition.Objectives[CurrentObjective];
	switch(Objective.Type)
	{
	case(ObjectiveType::DESTROYALLENEMIES):

		Targets = EnemyActors;
		break;

	case(ObjectiveType::DESTROYSPECIFICENEMIES):
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), Objective.ObjectiveTag, Targets);
		break;

	default:
		break;
	}
}
void APlaneGameMode::UpdateTargets(AActor* Actor) 
{
	FObjectiveDefinition Objective = MissionDefinition.Objectives[CurrentObjective];
	switch (Objective.Type)
	{
	case(ObjectiveType::DESTROYALLENEMIES):
		Targets.Remove(Actor);
		break;

	case(ObjectiveType::DESTROYSPECIFICENEMIES):
		if (Actor->ActorHasTag(Objective.ObjectiveTag))
		{
			Targets.Remove(Actor);
		}
		break;

	default:
		break;
	}

	if (Targets.IsEmpty())
	{
		JumpToNextObjective();
	}
}
void APlaneGameMode::SpawnActorsFromSpawnerTag(FName Tag) 
{
	TArray<AActor*>  selectedSpawners;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ASpawner::StaticClass(), Tag, selectedSpawners);
	for (AActor* spawner : selectedSpawners)
	{
		Cast<ASpawner>(spawner)->SpawnActor();
	}
}
void APlaneGameMode::ExitMission() 
{
	UGameplayStatics::OpenLevel(GetWorld(), "LV_MainMenu");
}