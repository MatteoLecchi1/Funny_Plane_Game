// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaneController.h"
#include "PlanePawn.h"
#include "Blueprint/UserWidget.h"
#include "PlanePlayerHUD.h"
#include "DeathScreen.h"

void APlaneController::BeginPlay()
{
	Super::BeginPlay();
	PlayerControlledPlane = Cast<APlanePawn>(this->GetPawn());
	Cast<UStaticMeshComponent>(PlayerControlledPlane->GetRootComponent())->SetSimulatePhysics(true);

	if (widgetHUDClass)
	{
		widgetHUDInstance = CreateWidget<UPlanePlayerHUD>(this, widgetHUDClass);
		if (widgetHUDInstance)
			widgetHUDInstance->AddToPlayerScreen();
	}
	if (widgetDeathScreenClass)
	{
		widgetDeathScreenInstance = CreateWidget<UDeathScreen>(this, widgetDeathScreenClass);
	}
}
void APlaneController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateHealth();
}
void APlaneController::UpdateHealth()
{
	if (widgetHUDInstance && PlayerControlledPlane)
	{
		widgetHUDInstance->UpdateHealth(PlayerControlledPlane->CurrentHealth, PlayerControlledPlane->MaxHealth);
	}
}

void APlaneController::OnControlledPlaneDeath()
{
	SetPause(true);
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	UnPossess();
	if (widgetDeathScreenInstance)
		widgetDeathScreenInstance->AddToPlayerScreen();
}