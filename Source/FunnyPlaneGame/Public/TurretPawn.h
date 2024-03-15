// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Hardpoint.h"
#include "TurretPawn.generated.h"

UCLASS()
class FUNNYPLANEGAME_API ATurretPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATurretPawn();

	UPROPERTY()
	TArray <UHardpoint*> Hardpoints;

	UPROPERTY()
	USceneComponent* turretBase;
	UPROPERTY()
	USceneComponent* turretGimball;
	
	UPROPERTY()
	double CurrentYaw = 0;
	UPROPERTY()
	double CurrentPitch = 0;
	UPROPERTY(EditAnywhere)
	double RotationSpeed = 1;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
