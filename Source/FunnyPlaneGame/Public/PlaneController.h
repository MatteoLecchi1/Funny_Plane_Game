// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlaneController.generated.h"

/**
 * 
 */
UCLASS()
class FUNNYPLANEGAME_API APlaneController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	class APlanePawn* PlayerControlledPlane;

	//UI
	//HUD
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UPlanePlayerHUD> widgetHUDClass;
	UPROPERTY()
	class UPlanePlayerHUD* widgetHUDInstance;
	//DeathScreen
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UDeathScreen> widgetDeathScreenClass;
	UPROPERTY()
	class UDeathScreen* widgetDeathScreenInstance;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UpdateHealthAndShield();
	void OnControlledPlaneDeath();

};
