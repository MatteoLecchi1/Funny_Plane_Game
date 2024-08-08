// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MissionDefinition.h"
#include "PlaneGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FUNNYPLANEGAME_API APlaneGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<AActor*> FriendlyActors;
	UPROPERTY()
	TArray<AActor*> EnemyActors;
	UPROPERTY()
	AActor* PlayerActor;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> EnemyMarkerWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> FriendlyMarkerWidget;

	UPROPERTY(EditAnywhere)
	FMissionDefinition MissionDefinition;
	UPROPERTY()
	int CurrentObjective;
	UPROPERTY()
	TArray<AActor*> Targets;

	void AddActorToArrays(AActor* Actor);
	void RemoveActorFromArrays(AActor* Actor);
	void JumpToNextObjective();

	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;
};
