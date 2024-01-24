// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlanePawn.generated.h"

UCLASS()
class FUNNYPLANEGAME_API APlanePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlanePawn();

	UPROPERTY(EditAnywhere)
	double RotationSpeed = 1.;

	UPROPERTY(EditAnywhere)
	double Speed = 100.;

	UPROPERTY(EditAnywhere)
	double PitchMultiplier = 1;
	UPROPERTY(EditAnywhere)
	double YawMultiplier = 1;
	UPROPERTY(EditAnywhere)
	double RollMultiplier = 1;

	UPROPERTY()
	double CurrentSteer = 0;

	UPROPERTY()
	double CurrentThrust = 0;
	double TargetThrust = 0;
	UPROPERTY(EditAnywhere)
	double MaxTargetThrust = 100;
	UPROPERTY(EditAnywhere)
	double MinTargetThrust = 20;

	UPROPERTY()
	double CurrentPitch = 0;

	UPROPERTY()
	double CurrentRoll = 0;

	UPROPERTY()
	FRotator Rotator = FRotator::ZeroRotator;


	UPROPERTY(EditAnywhere)
	TArray<FVector> hardpoints;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class AHardpointWeapon>> hardpointWeapons;
	TArray<TSubclassOf<class AHardpoint>> Hardpoints;

	bool Fire1;
	bool Fire2;
	bool Fire3;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ProcessPitch(float InPitch);
	void ProcessSteer(float InSteer);
	void ProcessThrust(float InThrust);
	void ProcessRoll(float InRoll);
	void ProcessFire1Pressed();
	void ProcessFire1Released();
	void ProcessFire2Pressed();
	void ProcessFire2Released();
	void ProcessFire3Pressed();
	void ProcessFire3Released();
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
