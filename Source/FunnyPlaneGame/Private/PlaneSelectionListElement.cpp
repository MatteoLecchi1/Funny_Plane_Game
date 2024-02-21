// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaneSelectionListElement.h"
#include "Components/TextBlock.h"
#include "PlanePawn.h"

void UPlaneSelectionListElement::UpdatePlane(APlanePawn* PlaneIn)
{
	Plane = PlaneIn;
}
void UPlaneSelectionListElement::UpdatePlaneName(FText NewName)
{
	PlaneName->SetText(NewName);
}
