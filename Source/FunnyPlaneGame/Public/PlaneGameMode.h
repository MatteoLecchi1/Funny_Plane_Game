// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
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
	void AddActorToArrays(AActor* Actor);
protected:
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;
};
