// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaneSelectionListElement.h"
#include "Components/TextBlock.h"


void UPlaneSelectionListElement::UpdatePlaneName(FText NewName)
{
	PlaneName->SetText(NewName);
}
