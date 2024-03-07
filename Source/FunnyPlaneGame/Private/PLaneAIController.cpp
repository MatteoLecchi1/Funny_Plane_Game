// Fill out your copyright notice in the Description page of Project Settings.


#include "PLaneAIController.h"
#include "PlanePawn.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void APLaneAIController::BeginPlay() 
{
	Super::BeginPlay();
}
void APLaneAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ControlledPlane = Cast<APlanePawn>(GetPawn());
	auto Component = Cast<UPrimitiveComponent>(ControlledPlane->GetRootComponent());
	Component->SetSimulatePhysics(true);

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "IsFriendly", AllTargets);
	//set steer
	auto currentTarget = AllTargets[0];
	FVector targetRelativePosition = currentTarget->GetActorLocation() - ControlledPlane->GetActorLocation();
	targetRelativePosition.Normalize();
	ControlledPlane->CurrentSteer = targetRelativePosition.Y;
	//set roll
	FVector targetRotation = currentTarget->GetActorRotation().Euler();
 	ControlledPlane->CurrentRoll= targetRotation.X/360;
}