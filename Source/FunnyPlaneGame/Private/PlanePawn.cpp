// Fill out your copyright notice in the Description page of Project Settings.


#include "PlanePawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Math/Rotator.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/UserWidget.h"
#include "PlanePlayerHUD.h"
#include "DeathScreen.h"
#include "Hardpoint.h"
#include "PlaneConfigurationSaveGame.h"

// Sets default values
APlanePawn::APlanePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APlanePawn::BeginPlay()
{
	Super::BeginPlay();

	CameraArmComponet = Cast<USpringArmComponent>(GetComponentByClass<USpringArmComponent>());

	CurrentHealth = MaxHealth;
	CurrentShield = MaxShield;

	GetComponents<UHardpoint>(Hardpoints, true);

	SetCanBeDamaged(true);

	if (!Configuration.PlaneKey.IsNone())
	{
		ApplyConfiguration(Configuration);
	}

	auto Component = Cast<UPrimitiveComponent>(GetRootComponent());
	Component->SetPhysicsLinearVelocity(Component->GetForwardVector() * 1000.f);
}

// Called every frame
void APlanePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto Component = Cast<UPrimitiveComponent>(GetRootComponent());
	if (!Component->IsSimulatingPhysics())
		return;
	
	if (bPhysicsMovement)
	{
		FTransform PlaneTransform = Component->GetComponentTransform();
		FVector CenterOfMass = Component->GetBodyInstance()->GetMassSpaceLocal().GetTranslation();

		FVector PlaneVelocity = Component->GetPhysicsLinearVelocity();
		FVector LocalPlaneVelocity = PlaneTransform.InverseTransformVector(PlaneVelocity);
		FVector Force = FVector::ZeroVector, Torque = FVector::ZeroVector;

		auto AddWingLocalForce = [&](FVector WingPosition, FVector WingNormal, double WingCoefficient)
		{
			double ForceMagnitude = -FVector::DotProduct(WingNormal, LocalPlaneVelocity) * LocalPlaneVelocity.Size() * WingCoefficient;
			FVector AppliedForce = WingNormal * ForceMagnitude;
			Force += AppliedForce;
			Torque += FVector::CrossProduct(WingPosition - CenterOfMass, AppliedForce);
		};

		double WingPitchAngle = PhysicsParams.WingControlAngles.Pitch * -CurrentPitch;
		double WingRollAngle = PhysicsParams.WingControlAngles.Roll * CurrentRoll;
		double RudderAngle = PhysicsParams.WingControlAngles.Yaw * CurrentSteer;

		FVector LeftWingDirection = FRotator(WingRollAngle, 0., 0.).RotateVector(FVector::UpVector);
		FVector RightWingDirection = FRotator(-WingRollAngle, 0., 0.).RotateVector(FVector::UpVector);
		FVector RearWingDirection = FRotator(WingPitchAngle, 0., 0.).RotateVector(FVector::UpVector);
		FVector RudderDirection = FRotator(0., RudderAngle, 0.).RotateVector(FVector::RightVector);

		AddWingLocalForce(FVector(0., -PhysicsParams.WingOffset, 0.), LeftWingDirection, PhysicsParams.WingLiftCoefficient);
		AddWingLocalForce(FVector(0., PhysicsParams.WingOffset, 0.), RightWingDirection, PhysicsParams.WingLiftCoefficient);

		AddWingLocalForce(FVector(0., 0., -PhysicsParams.WingOffset), FVector::RightVector, PhysicsParams.WingLiftCoefficient);
		AddWingLocalForce(FVector(0., 0., PhysicsParams.WingOffset), FVector::RightVector, PhysicsParams.WingLiftCoefficient);

		AddWingLocalForce(FVector(PhysicsParams.RudderOffset, 0., 0.), RearWingDirection, PhysicsParams.RearWingLiftCoefficient);
		AddWingLocalForce(FVector(PhysicsParams.RudderOffset, 0., 0.), RudderDirection, PhysicsParams.WingRudderCoefficient);

		PhysicsParams.CurrentThrustForce += CurrentThrust * DeltaTime * PhysicsParams.ThrustForceVariation;
		PhysicsParams.CurrentThrustForce = FMath::Clamp(PhysicsParams.CurrentThrustForce, PhysicsParams.MinThrustForce, PhysicsParams.MaxThrustForce);

		if (!IsAOAOn)
		{
			Force.X += PhysicsParams.CurrentThrustForce;
		}

		Force += LocalPlaneVelocity * (-PhysicsParams.AirDragFactor * LocalPlaneVelocity.Size());

		Component->AddForce(PlaneTransform.TransformVector(Force));
		Component->AddTorqueInRadians(PlaneTransform.TransformVector(Torque));
	}
	else
	{
		// rotates the plane dependant on CurrentPitch,CurrentSteer and CurrentRoll
		if (!IsAOAOn)
		{
			//without AOA
			//Pitch
			Component->AddLocalRotation(FRotator(RotationSpeed * CurrentPitch * DeltaTime * PitchMultiplier, 0, 0));
			//Yaw
			Component->AddLocalRotation(FRotator(0, RotationSpeed * CurrentSteer * DeltaTime * YawMultiplier, 0));
			//Roll
			Component->AddLocalRotation(FRotator(0, 0, RotationSpeed * CurrentRoll * DeltaTime * RollMultiplier));
		}
		else
		{
			//with AOA
			//Pitch
			Component->AddLocalRotation(FRotator(RotationSpeed * CurrentPitch * DeltaTime * PitchMultiplier * AOARotationMultiplier, 0, 0));
			//Yaw
			Component->AddLocalRotation(FRotator(0, RotationSpeed * CurrentSteer * DeltaTime * YawMultiplier * AOARotationMultiplier, 0));
			//Roll
			Component->AddLocalRotation(FRotator(0, 0, RotationSpeed * CurrentRoll * DeltaTime * RollMultiplier * AOARotationMultiplier));
		}

		//Clamp throttle
		TargetThrust += CurrentThrust * DeltaTime;
		TargetThrust = FMath::Clamp(TargetThrust, MinTargetThrust, MaxTargetThrust);
	}
	
	if (IsCameraLockedOn) 
	{
		if (LockedOnActor->IsValidLowLevel()) 
		{
			CameraArmComponet->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), LockedOnActor->GetActorLocation()));
		}
		else
		{
			IsCameraLockedOn = false;
			TargetCameraX = 0.f;
			TargetCameraY = 0.f; 
			TargetCameraRotation = FVector(0.f, (float)TargetCameraY, (float)TargetCameraX);
			CameraArmComponet->SetRelativeRotation(FRotator::MakeFromEuler(TargetCameraRotation));
		}
	}
	else
	{
		if (!IsAOAOn)
		{
			if (CurrenCameraX != 0.f || CurrenCameraY != 0.f) {
				//rotate camera dependant on cameraX and cameraY
				TargetCameraX += CurrenCameraX * CameraSpeedX;
				TargetCameraX = FMath::Clamp(TargetCameraX, -175.f, 175.f);

				TargetCameraY += CurrenCameraY * CameraSpeedY;
				TargetCameraY = FMath::Clamp(TargetCameraY, -175.f, 175.f);

				TimeSinceLastCameraInput = 0.f;
			}
			else
			{
				//reset camera rotation if enoght time has passed
				TimeSinceLastCameraInput += DeltaTime;
				if (TimeSinceLastCameraInput > TimeCameraToReset) {
					TargetCameraX = 0.f;
					TargetCameraY = 0.f;
				}
			}

			TargetCameraRotation = FVector(0.f, (float)TargetCameraY, (float)TargetCameraX);
			CameraArmComponet->SetRelativeRotation(FRotator::MakeFromEuler(TargetCameraRotation));
		}
		else 
		{
			if(AOALocksCamera)
				CameraArmComponet->SetWorldRotation(CameraRotationOnAOABegin);
		}
	}
	if (!IsAOAOn)
	{
		if (!bPhysicsMovement)
		{
			// Add a force dependent on Thrust in the forward direction
			Component->SetAllPhysicsLinearVelocity(Component->GetForwardVector() * (TargetThrust / MaxTargetThrust) * Speed);
		}
	}

	RechargeShield(DeltaTime);
}

// Called to bind functionality to input
void APlanePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("Steer", this, &APlanePawn::ProcessSteer);
	InputComponent->BindAxis("Thrust", this, &APlanePawn::ProcessThrust);
	InputComponent->BindAxis("Pitch", this, &APlanePawn::ProcessPitch);
	InputComponent->BindAxis("Roll", this, &APlanePawn::ProcessRoll);
	InputComponent->BindAxis("CameraX", this, &APlanePawn::ProcessCameraX);
	InputComponent->BindAxis("CameraY", this, &APlanePawn::ProcessCameraY);
	InputComponent->BindAction("Fire1", IE_Pressed, this, &APlanePawn::ProcessFire1Pressed);
	InputComponent->BindAction("Fire1", IE_Released, this, &APlanePawn::ProcessFire1Released);
	InputComponent->BindAction("Fire2", IE_Pressed, this, &APlanePawn::ProcessFire2Pressed);
	InputComponent->BindAction("Fire2", IE_Released, this, &APlanePawn::ProcessFire2Released);
	InputComponent->BindAction("Fire3", IE_Pressed, this, &APlanePawn::ProcessFire3Pressed);
	InputComponent->BindAction("Fire3", IE_Released, this, &APlanePawn::ProcessFire3Released);
	InputComponent->BindAction("Evade", IE_Pressed, this, &APlanePawn::ProcessEvadePressed);
	InputComponent->BindAction("Evade", IE_Released, this, &APlanePawn::ProcessEvadeReleased);
	InputComponent->BindAction("LockOn", IE_Pressed, this, &APlanePawn::ProcessLockOnPressed);
}

// Configuration management
void APlanePawn::ApplyConfiguration(FSavedPlane& SavedPlane)
{
	Configuration = SavedPlane;

	for (int i = 0; i < SavedPlane.SavedHardpointWeapons.Num(); i++)
	{
		Hardpoints[i]->AssignWeapon(SavedPlane.SavedHardpointWeapons[i]);
	}
}

//damage management
float APlanePawn::TakeDamage(float DamageAmount,struct FDamageEvent const& DamageEvent,class AController* EventInstigator,AActor* DamageCauser) 
{
	TimeSinceDamageTaken = 0;
	bool ShieldWasOn = false;
	if (CurrentShield > 0)
		ShieldWasOn = true;

	CurrentShield -= DamageAmount;

	if (CurrentShield < 0) 
	{
		CurrentHealth -= abs(CurrentShield);
		CurrentShield = 0;
		if (ShieldWasOn)
			OnShieldBreak();
		
	}
	//if players health is less than 0
	if (CurrentHealth <= 0)
		OnPlayerDeath();

	return DamageAmount;
}
void APlanePawn::OnPlayerDeath()
{
	auto PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PC->SetPause(true);
	PC->bShowMouseCursor = true;
	PC->bEnableClickEvents = true;
	PC->bEnableMouseOverEvents = true;
}
void APlanePawn::OnShieldBreak() 
{

}
void APlanePawn::RechargeShield(float DeltaTime)
{
	TimeSinceDamageTaken += DeltaTime;
	if (TimeSinceDamageTaken > TimeBeforeShieldStartsRecover && CurrentShield < MaxShield) 
	{
		CurrentShield += ShieldRecoverPerSecond * DeltaTime;
	}
}
// input processing
void APlanePawn::ProcessThrust(float InThrust)
{
	CurrentThrust = InThrust;
}
void APlanePawn::ProcessPitch(float InPitch)
{
	CurrentPitch = InPitch;
}
void APlanePawn::ProcessRoll(float InRoll)
{
	CurrentRoll = InRoll;
}
void APlanePawn::ProcessSteer(float InSteer)
{
	CurrentSteer = InSteer;
}
void APlanePawn::ProcessCameraX(float InCameraX)
{
	CurrenCameraX = InCameraX;
}
void APlanePawn::ProcessCameraY(float InCameraY)
{
	CurrenCameraY = InCameraY;
}
void APlanePawn::ProcessFire1Pressed()
{
	for (UHardpoint* a : Hardpoints) 
	{
		if (a->thisShootButton == ShootButton::DOWN)
			a->IsShooting=true;
	}
}
void APlanePawn::ProcessFire1Released()
{
	{
		for (UHardpoint* a : Hardpoints)
		{
			if (a->thisShootButton == ShootButton::DOWN)
				a->IsShooting = false;
		}
	}
}
void APlanePawn::ProcessFire2Pressed()
{
	for (UHardpoint* a : Hardpoints)
	{
		if (a->thisShootButton == ShootButton::LEFT)
			a->IsShooting = true;
	}
}
void APlanePawn::ProcessFire2Released()
{
	for (UHardpoint* a : Hardpoints)
	{
		if (a->thisShootButton == ShootButton::LEFT)
			a->IsShooting = false;
	}
}
void APlanePawn::ProcessFire3Pressed()
{
	for (UHardpoint* a : Hardpoints)
	{
		if (a->thisShootButton == ShootButton::UP)
			a->IsShooting = true;
	}
}
void APlanePawn::ProcessFire3Released()
{
	for (UHardpoint* a : Hardpoints)
	{
		if (a->thisShootButton == ShootButton::UP)
			a->IsShooting = false;
	}
}
void APlanePawn::ProcessEvadePressed()
{
	IsAOAOn = true;
	CameraRotationOnAOABegin = CameraArmComponet->GetComponentRotation();
}
void APlanePawn::ProcessEvadeReleased()
{
	IsAOAOn = false;
}
void APlanePawn::ProcessLockOnPressed()
{
	if (!IsCameraLockedOn) 
	{
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), "IsEnemy", AllEnemiesInMap);
		for (AActor* a : AllEnemiesInMap)
		{
			if (ClosestEnemyInMapDistace < (a->GetActorLocation() - GetActorLocation()).Length())
				ClosestEnemyInMapDistace = (a->GetActorLocation() - GetActorLocation()).Length();
			ClosestEnemyInMap = a;
		}
		LockedOnActor = ClosestEnemyInMap;
		IsCameraLockedOn = true;
	}
	else
		IsCameraLockedOn = false;
}
