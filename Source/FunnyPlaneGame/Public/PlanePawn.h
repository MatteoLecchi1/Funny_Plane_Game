// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Hardpoint.h"
#include "PlanePawn.generated.h"

USTRUCT(BlueprintType)
struct FPlanePhysicsParams
{
	GENERATED_BODY()

	FPlanePhysicsParams() = default;

	UPROPERTY(EditAnywhere)
	double WingOffset = 1000.f;

	UPROPERTY(EditAnywhere)
	double RudderOffset = -1000.f;

	UPROPERTY(EditAnywhere)
	FRotator WingControlAngles = FRotator(5., 5., 5.);

	/* Affects roll */
	UPROPERTY(EditAnywhere)
	double WingLiftCoefficient = .01;

	/* Affects pitch */
	UPROPERTY(EditAnywhere)
	double RearWingLiftCoefficient = .01;

	/* Affects yaw */
	UPROPERTY(EditAnywhere)
	double WingRudderCoefficient = .01;

	UPROPERTY(EditAnywhere)
	double AirDragFactor = .000001;

	UPROPERTY()
	double CurrentThrustForce = 800000.;

	UPROPERTY(EditAnywhere)
	double MaxThrustForce = 1000000.;
	UPROPERTY(EditAnywhere)
	double MinThrustForce = 300000.;
	UPROPERTY(EditAnywhere)
	double ThrustForceVariation = 10000.;
};

UCLASS()
class FUNNYPLANEGAME_API APlanePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlanePawn();

	//Movement
	UPROPERTY(EditAnywhere, Category = "Movement")
	bool bPhysicsMovement = false;

	UPROPERTY(EditAnywhere, Category = "Movement")
	FPlanePhysicsParams PhysicsParams;

	UPROPERTY(EditAnywhere, Category = "Movement")
	double RotationSpeed = 1.;

	UPROPERTY(EditAnywhere, Category = "Movement")
	double Speed = 100.;
	UPROPERTY()
	double CurrentThrust = 0;
	double TargetThrust = 0;
	UPROPERTY(EditAnywhere, Category = "Movement")
	double MaxTargetThrust = 100;
	UPROPERTY(EditAnywhere, Category = "Movement")
	double MinTargetThrust = 20;

	UPROPERTY(EditAnywhere, Category = "Movement")
	double PitchMultiplier = 1;
	UPROPERTY(EditAnywhere, Category = "Movement")
	double YawMultiplier = 1;
	UPROPERTY(EditAnywhere, Category = "Movement")
	double RollMultiplier = 1;

	UPROPERTY()
	double CurrentSteer = 0;
	UPROPERTY()
	double CurrentPitch = 0;
	UPROPERTY()
	double CurrentRoll = 0;
	UPROPERTY()
	bool IsAOAOn = false;
	UPROPERTY(EditAnywhere, Category = "Movement")
	double AOARotationMultiplier = 1;
	//Camera
	UPROPERTY()
	class USpringArmComponent* CameraArmComponet;
	UPROPERTY()
	FRotator CameraRotationOnAOABegin = FRotator::ZeroRotator;;
	UPROPERTY(EditAnywhere)
	bool AOALocksCamera = false;
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

	const float& NearestActorSearchRange = 1000.f;
	UPROPERTY()
	TArray<AActor*> AllEnemiesInMap;
	UPROPERTY()
	AActor* ClosestEnemyInMap;
	float ClosestEnemyInMapDistace= std::numeric_limits<float>::max();

	//Combat
	UPROPERTY(EditAnywhere)
	double MaxHealth = 100.;
	UPROPERTY()
	double CurrentHealth = 1.;
	UPROPERTY(EditAnywhere)
	double MaxShield = 100.;
	UPROPERTY()
	double CurrentShield = 1.;
	UPROPERTY()
	double TimeSinceDamageTaken = 0.;
	UPROPERTY(EditAnywhere)
	double TimeBeforeShieldStartsRecover = 5.;
	UPROPERTY(EditAnywhere)
	double ShieldRecoverPerSecond = 50.;

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

	UPROPERTY()
	TArray <UHardpoint*> hardpoints;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void AddWingForce(FVector WingPosition, FVector WingNormal, double WingCoefficient);

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
	void ProcessEvadePressed();
	void ProcessEvadeReleased();
	void ProcessLockOnPressed();
	void UpdateHealthAndShield();
	void RechargeShield(float DeltaTime);
	void OnShieldBreak();
	void OnPlayerDeath();
	
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
