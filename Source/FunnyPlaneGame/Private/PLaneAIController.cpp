// Fill out your copyright notice in the Description page of Project Settings.


#include "PLaneAIController.h"
#include "PlanePawn.h"

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
	ControlledPlane->CurrentSteer = 0.2;
}