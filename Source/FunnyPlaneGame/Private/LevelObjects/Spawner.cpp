// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelObjects/Spawner.h"
#include "TimerManager.h"

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();

	FTimerDelegate SpawnDelegate;
	SpawnDelegate.BindUFunction(this, FName("SpawnActor"));
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, SpawnDelegate, DelaybetweenSpawns, false);
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
void ASpawner::SpawnActor()
{
	FTimerDelegate SpawnDelegate;
	SpawnDelegate.BindUFunction(this, FName("SpawnActor"));
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, SpawnDelegate, DelaybetweenSpawns, false);
	if (ShouldSpawn) 
	{
		APlanePawn* PlaneInstance = GetWorld()->SpawnActor<APlanePawn>(SpawnClass, GetActorLocation(), GetActorRotation());
		PlaneInstance->SpawnDefaultController();
	}
}

