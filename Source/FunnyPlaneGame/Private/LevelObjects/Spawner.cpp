// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelObjects/Spawner.h"
#include "TimerManager.h"
#include "PlaneGameMode.h"

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
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
void ASpawner::SpawnActor()
{
	if (IsContinuousSpawn)
	{
		TimerHandle;
		FTimerDelegate Delegate;

		Delegate.BindUFunction(this, FName("SpawnActor"));
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, Delegate, DelaybetweenSpawns, true);
	}
	else 
	{
		SpawnSingleActor();
	}
}
void ASpawner::SpawnSingleActor()
{
	AActor* SpawnedInstance = GetWorld()->SpawnActor<APlanePawn>(SpawnClass, GetActorLocation(), GetActorRotation());
	if (APlanePawn* PlaneInstance = Cast<APlanePawn>(SpawnedInstance)) {
		PlaneInstance->SpawnDefaultController();
	}
	if (ATurretPawn* TurretInstance = Cast<ATurretPawn>(SpawnedInstance)) {
		TurretInstance->SpawnDefaultController();
	}
}
void ASpawner::SpawnSingleActorAndAttach(AActor* Actor)
{
	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FTransform spawnTransform = RootComponent->GetComponentTransform();

	FString TheFloatStr = FString::FromInt(Cast<APlaneGameMode>(GetWorld()->GetAuthGameMode())->CurrentObjective);

	AActor* SpawnedInstance = GetWorld()->SpawnActor<AActor>(SpawnClass, spawnTransform.GetLocation(), spawnTransform.GetRotation().Rotator(), ActorSpawnParameters);

	TheFloatStr = FString::FromInt(Cast<APlaneGameMode>(GetWorld()->GetAuthGameMode())->CurrentObjective);

	if (APlanePawn* PlaneInstance = Cast<APlanePawn>(SpawnedInstance)) {
		PlaneInstance->SpawnDefaultController();
	}
	if (ATurretPawn* TurretInstance = Cast<ATurretPawn>(SpawnedInstance)) {
		TurretInstance->SpawnDefaultController();
	}
	if (Actor != nullptr) {
		SpawnedInstance->AttachToActor(Actor, FAttachmentTransformRules::KeepWorldTransform);
	}
}
