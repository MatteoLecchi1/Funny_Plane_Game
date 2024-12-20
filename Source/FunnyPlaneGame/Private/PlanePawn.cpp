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
#include "PlaneGameMode.h"
#include "PlaneController.h"
#include "Projectile.h"
#include "NiagaraFunctionLibrary.h"

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
	LockedEnemyArrowComponet = Cast<USceneComponent>(GetComponentsByTag(USceneComponent::StaticClass(), "LockedArrow")[0]);

	CurrentHealth = MaxHealth;

	GetComponents<UHardpoint>(Hardpoints, true);

	SetCanBeDamaged(true);

	if (!Configuration.PlaneKey.IsNone())
	{
		ApplyConfiguration(Configuration);
	}

	auto Component = Cast<UPrimitiveComponent>(GetRootComponent());
	Component->SetPhysicsLinearVelocity(Component->GetForwardVector() * 1000.f);

	Cast<UStaticMeshComponent>(Component)->OnComponentHit.AddDynamic(this, &APlanePawn::OnHit);

	LockedEnemyArrowComponet->SetVisibility(false, true);
	
	//add this actor to gamemode lists
	auto gamemode = Cast<APlaneGameMode>(GetWorld()->GetAuthGameMode());
	if (gamemode) 
	{
		if (this == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
		{
			gamemode->PlayerActor = this;
			if(CrosshairClass)
				CosshairInstance = GetWorld()->SpawnActor<AActor>(CrosshairClass, GetActorLocation() + GetActorForwardVector()*50000, GetActorRotation());
		}
		else
		{
			gamemode->AddActorToArrays(this);
		}
	}
}

// Called every frame
void APlanePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ManageMovement(DeltaTime);

	//if this is player
	if (this == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		ManageCamera(DeltaTime);

		if(CosshairInstance)
			ManageCrosshair();

		for (UHardpoint* a : Hardpoints)
		{
			if (IsDownFireing && a->thisShootButton == ShootButton::DOWN)
			{
				a->ShootWeapon(LockedOnActor);
			}
			else if(IsLeftFireing && a->thisShootButton == ShootButton::LEFT)
			{
				a->ShootWeapon(LockedOnActor);
			}
			else if(IsUpFireing && a->thisShootButton == ShootButton::UP)
			{
				a->ShootWeapon(LockedOnActor);
			}
		}
	}
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
	InputComponent->BindAction("LockOn", IE_Released, this, &APlanePawn::ProcessLockOnReleased);
	InputComponent->BindAction("LockOnNext", IE_Pressed, this, &APlanePawn::ProcessLockOnNextPressed);
	InputComponent->BindAction("LockOnPrev", IE_Pressed, this, &APlanePawn::ProcessLockOnPrevPressed);
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
		CurrentHealth -= DamageAmount;
		
	//if players health is less than 0
	if (CurrentHealth <= 0)
		OnPlayerDeath();

	return DamageAmount;
}
void APlanePawn::OnPlayerDeath()
{
	auto gamemode = Cast<APlaneGameMode>(GetWorld()->GetAuthGameMode());
	//remove this actor from gamemode lists
	if (gamemode)
	{
		if (this == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
		{
			gamemode->PlayerActor = nullptr; auto PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (auto PPC = Cast<APlaneController>(PC))
			{
				PPC->OnControlledPlaneDeath();
			}
		}
		else
		{
			gamemode->RemoveActorFromArrays(this);
		}
	}
	Destroy();
	if (ExplosionEffect) {
		UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), FRotator(0.f), FVector(1));
	}
	if (CosshairInstance) {
		CosshairInstance->Destroy();
	}
}
void APlanePawn::ManageCamera(float DeltaTime)
{
	auto GameMode = Cast<APlaneGameMode>(GetWorld()->GetAuthGameMode());
	if (!IsValid(LockedOnActor) && GameMode->EnemyActors.Num()>0)
	{
		//assign LockedOnActor if it's not assigned arleady
		ClosestEnemyInMapDistace = std::numeric_limits<float>::max();

		for (AActor* a : GameMode->EnemyActors)
		{
			if(IsValid(a))
			{
				if (ClosestEnemyInMapDistace > (a->GetActorLocation() - GetActorLocation()).Length())
				{
					ClosestEnemyInMapDistace = (a->GetActorLocation() - GetActorLocation()).Length();
					ClosestEnemyInMap = a;
				}
			}
		}
		LockedOnActor = ClosestEnemyInMap;
	}
	if (IsCameraLockedOn)
	{
		if (IsValid(LockedOnActor))
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
		if (IsValid(LockedOnActor) && LockedEnemyArrowComponet->IsVisible())
		{
			LockedEnemyArrowComponet->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), LockedOnActor->GetActorLocation()));
		}
		else
		{
			LockedEnemyArrowComponet->SetVisibility(false, true);
		}

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
				if (TimeSinceLastCameraInput > TimeCameraToReset) {
					TargetCameraX = 0.f;
					TargetCameraY = 0.f;
				}
			}

			TimeSinceLastCameraInput += DeltaTime;

			TargetCameraRotation = FVector(0.f, (float)TargetCameraY, (float)TargetCameraX);
			CameraArmComponet->SetRelativeRotation(FRotator::MakeFromEuler(TargetCameraRotation));
		}
		else
		{
			if (AOALocksCamera)
				CameraArmComponet->SetWorldRotation(CameraRotationOnAOABegin);
		}
	}
}
void APlanePawn::ManageCrosshair()
{
	FVector TraceStart = GetActorLocation();
	FVector TraceEnd = TraceStart + GetActorForwardVector() * 50000;

	//add ignored actors
	FCollisionQueryParams QueryParams;

	QueryParams.AddIgnoredActor(this);

	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, TraceChannelProperty, QueryParams);
	if (Hit.bBlockingHit)
	{
		CosshairInstance->SetActorLocation(Hit.ImpactPoint);
	}
	else 
	{
		CosshairInstance->SetActorLocation(TraceEnd);
	}
}
void APlanePawn::ManageMovement(float DeltaTime) 
{
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
		double WingPitchAngle;
		double WingRollAngle;
		double RudderAngle;
		if (!IsAOAOn)
		{
			WingPitchAngle = PhysicsParams.WingControlAngles.Pitch * -CurrentPitch;
			WingRollAngle = PhysicsParams.WingControlAngles.Roll * CurrentRoll;
			RudderAngle = PhysicsParams.WingControlAngles.Yaw * CurrentSteer;
		}
		else
		{
			WingPitchAngle = PhysicsParams.WingControlAngles.Pitch * -CurrentPitch * AOARotationMultiplier;
			WingRollAngle = PhysicsParams.WingControlAngles.Roll * CurrentRoll * AOARotationMultiplier;
			RudderAngle = PhysicsParams.WingControlAngles.Yaw * CurrentSteer * AOARotationMultiplier;
		}

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

		PhysicsParams.CurrentThrustForce = CurrentThrust * PhysicsParams.ThrustForceVariation + PhysicsParams.MinThrustForce;
		
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
	IsDownFireing = true;
}
void APlanePawn::ProcessFire1Released()
{
	IsDownFireing = false;
}
void APlanePawn::ProcessFire2Pressed()
{
	IsLeftFireing = true;
}
void APlanePawn::ProcessFire2Released()
{
	IsLeftFireing = false;
}
void APlanePawn::ProcessFire3Pressed()
{
	IsDownFireing = true;
}
void APlanePawn::ProcessFire3Released()
{
	IsDownFireing = false;
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
	TimeSinceLastCameraInput = 99.f;
 	if(!IsCameraLockAToggle)
	{
		IsCameraLockedOn = true;
		LockedEnemyArrowComponet->SetVisibility(false, true);
	}
	else 
	{
		if (IsCameraLockedOn)
		{

			IsCameraLockedOn = false;
			LockedEnemyArrowComponet->SetVisibility(true, true);
		}
		else
		{
			IsCameraLockedOn = true;
			LockedEnemyArrowComponet->SetVisibility(false, true);
		}
	}
}
void APlanePawn::ProcessLockOnReleased() 
{
	if(!IsCameraLockAToggle)
	{
		IsCameraLockedOn = false;
		LockedEnemyArrowComponet->SetVisibility(true, true);
	}
}
void APlanePawn::ProcessLockOnNextPressed()
{
	auto GameMode = Cast<APlaneGameMode>(GetWorld()->GetAuthGameMode());
	if (IsValid(LockedOnActor) && GameMode->EnemyActors.Num() > 1)
	{
		int EnemyInArrayIndex;
		GameMode->EnemyActors.Find(LockedOnActor, EnemyInArrayIndex);

		EnemyInArrayIndex++;

		if (EnemyInArrayIndex >= GameMode->EnemyActors.Num())
			EnemyInArrayIndex = 0;

		LockedOnActor = GameMode->EnemyActors[EnemyInArrayIndex];
	}
}
void APlanePawn::ProcessLockOnPrevPressed()
{
	auto GameMode = Cast<APlaneGameMode>(GetWorld()->GetAuthGameMode());
	if (IsValid(LockedOnActor) && GameMode->EnemyActors.Num() > 1)
	{
		int EnemyInArrayIndex;
		GameMode->EnemyActors.Find(LockedOnActor, EnemyInArrayIndex);

		EnemyInArrayIndex--;

		if (EnemyInArrayIndex < 0)
			EnemyInArrayIndex = GameMode->EnemyActors.Num() - 1;

		LockedOnActor = GameMode->EnemyActors[EnemyInArrayIndex];
	}
}

void APlanePawn::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) 
{
	if (HitComponent->GetOwner()->StaticClass() != AProjectile::StaticClass())
	{
		this->OnPlayerDeath();
	}
}
