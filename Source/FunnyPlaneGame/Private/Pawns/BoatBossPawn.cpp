// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/BoatBossPawn.h"
#include "LevelObjects/Spawner.h"

void ABoatBossPawn::OnJumpToNextObjective(int CurrentObjective)
{
	switch (CurrentObjective)
	{
	case(0):
		break;
	case(1):
		StartFlight = true;
		SpawnNewWeapons();
		break;

	case(2):
		break;

	default:
		break;
	}
}
void ABoatBossPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FTransform AddTransform = FTransform(FVector(HorizontalSpeed * DeltaTime, 0, 0));
	if (StartFlight == true && GetActorLocation().Z < TargetAltitude)
	{
		AddTransform += FTransform(FVector(0,0,VerticalSpeed * DeltaTime));
	}
	AddActorWorldTransform(AddTransform);
}
void ABoatBossPawn::SpawnNewWeapons()
{
	TArray <AActor*> AllChildren;
	GetAllChildActors(AllChildren, false);
	for (AActor* Child : AllChildren)
	{
		if (ASpawner* spawner = Cast<ASpawner>(Child))
		{
				spawner->SpawnSingleActorAndAttach(this);
		}
	}
}