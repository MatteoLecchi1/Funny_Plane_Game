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
	auto currentTarget = AllTargets[0];

	//set steer and pitch   
	FVector targetRelativePosition = ControlledPlane->GetActorTransform().InverseTransformPosition(currentTarget->GetActorLocation());
	FRotator targetRotation = targetRelativePosition.Rotation();
	ControlledPlane->CurrentSteer = FMath::GetMappedRangeValueClamped(FVector2D(-30., 30), FVector2D(1., -1.), targetRotation.Yaw);
	ControlledPlane->CurrentPitch = FMath::GetMappedRangeValueClamped(FVector2D(-30., 30), FVector2D(-1., 1.), targetRotation.Pitch);
}