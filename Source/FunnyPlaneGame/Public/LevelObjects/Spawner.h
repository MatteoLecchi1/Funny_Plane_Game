// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlanePawn.h"
#include "TurretPawn.h"
#include "TimerManager.h"
#include "Spawner.generated.h"

UCLASS()
class FUNNYPLANEGAME_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawner();
	
	UPROPERTY(EditAnywhere)
	float DelaybetweenSpawns = 50;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> SpawnClass;
	UPROPERTY(EditAnywhere)
	bool IsContinuousSpawn = false;
	UPROPERTY()
	FTimerHandle TimerHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void SpawnActor(); 
	UFUNCTION()
	void SpawnSingleActor();
	UFUNCTION()
	void SpawnSingleActorAndAttach(AActor* Actor);
};
