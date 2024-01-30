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
	double MaxHealth = 100.;
	UPROPERTY()
	double CurrentHealth = 1.;
	UPROPERTY(EditAnywhere)
	double MaxShield = 100.;
	UPROPERTY()
	double CurrentShield = 1.;

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
	double CurrentThrust = 0;
	double TargetThrust = 0;
	UPROPERTY(EditAnywhere)
	double MaxTargetThrust = 100;
	UPROPERTY(EditAnywhere)
	double MinTargetThrust = 20;

	UPROPERTY()
	double CurrentSteer = 0;
	UPROPERTY()
	double CurrentPitch = 0;
	UPROPERTY()
	double CurrentRoll = 0;

	UPROPERTY()
	double CurrenCameraX = 0;
	UPROPERTY()
	double TargetCameraX = 0;
	UPROPERTY()
	double CurrenCameraY = 0;
	UPROPERTY()
	double TargetCameraY = 0;
	UPROPERTY(EditAnywhere)
	double CameraSpeedX = 1;
	UPROPERTY(EditAnywhere)
	double CameraSpeedY = 1;

	UPROPERTY()
	FVector TargetCameraRotation = FVector::ZeroVector;

	float TimeSinceLastCameraInput = 100.f;
	UPROPERTY(EditAnywhere)
	float TimeCameraToReset = 2.f;

	UPROPERTY()
	bool IsCameraLockedOn = false;
	AActor* LockedOnActor =this;

	UPROPERTY()
	FRotator Rotator = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class AHardpointWeapon>> hardpointWeapons;

	const float& NearestActorSearchRange = 1000.f;
	TArray<AActor*> AllEnemiesInMap;
	AActor* ClosestEnemyInMap;
	float ClosestEnemyInMapDistace= std::numeric_limits<float>::max();


	bool Fire1 = false;
	bool Fire2 = false;
	bool Fire3 = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ProcessPitch(float InPitch);
	void ProcessSteer(float InSteer);
	void ProcessThrust(float InThrust);
	void ProcessRoll(float InRoll);
	void ProcessCameraX(float InCameraX);
	void ProcessCameraY(float InCameray);
	void ProcessFire1Pressed();
	void ProcessFire1Released();
	void ProcessFire2Pressed();
	void ProcessFire2Released();
	void ProcessFire3Pressed();
	void ProcessFire3Released();
	void ProcessLockOnPressed();
	void OnShieldBreak();
	
	virtual float TakeDamage
	(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	)override;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
