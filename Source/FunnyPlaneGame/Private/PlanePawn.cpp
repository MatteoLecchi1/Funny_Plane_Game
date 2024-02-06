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

	CurrentHealth = MaxHealth;
	CurrentShield = MaxShield;
	UpdateHealthAndShield();

	GetComponents<UHardpoint>(hardpoints, true);

	if (IsLocallyControlled() && widgetHUDClass)
	{
		widgetHUDInstance = CreateWidget<UPlanePlayerHUD>(UGameplayStatics::GetPlayerController(GetWorld(),0), widgetHUDClass);
		if(widgetHUDInstance)
			widgetHUDInstance->AddToPlayerScreen();
	}
	if (IsLocallyControlled() && widgetDeathScreenClass)
	{
		widgetDeathScreenInstance = CreateWidget<UDeathScreen>(UGameplayStatics::GetPlayerController(GetWorld(), 0), widgetDeathScreenClass);
	}
	SetCanBeDamaged(true);
}

// Called every frame
void APlanePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto Component = Cast<UPrimitiveComponent>(GetRootComponent());
	auto CameraArmComponet = Cast<USpringArmComponent>(GetComponentByClass<USpringArmComponent>());

	// rotates the plane dependant on CurrentPitch,CurrentSteer and CurrentRoll
	if (!IsAOAOn) {
		//without AOA
		//Pitch
		Component->AddLocalRotation(FRotator(RotationSpeed * CurrentPitch * DeltaTime * PitchMultiplier, 0, 0));
		//Yaw
		Component->AddLocalRotation(FRotator(0, RotationSpeed * CurrentSteer * DeltaTime * YawMultiplier, 0));
		//Roll
		Component->AddLocalRotation(FRotator(0, 0, RotationSpeed * CurrentRoll * DeltaTime * RollMultiplier));
	}
	else {
		//with AOA
		//Pitch
		Component->AddLocalRotation(FRotator(RotationSpeed * CurrentPitch * DeltaTime * PitchMultiplier * AOARotationMultiplier, 0, 0));
		//Yaw
		Component->AddLocalRotation(FRotator(0, RotationSpeed * CurrentSteer * DeltaTime * YawMultiplier * AOARotationMultiplier, 0));
		//Roll
		Component->AddLocalRotation(FRotator(0, 0, RotationSpeed * CurrentRoll * DeltaTime * RollMultiplier * AOARotationMultiplier));
	}

	//Clamp throttle
	TargetThrust += CurrentThrust*DeltaTime;
	TargetThrust = FMath::Clamp(TargetThrust, MinTargetThrust, MaxTargetThrust);
	
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
	if (!IsAOAOn) {
		// Add a force dependent on Thrust in the forward direction
		Component->SetAllPhysicsLinearVelocity(Component->GetForwardVector() * (TargetThrust / MaxTargetThrust) * Speed);
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
	UpdateHealthAndShield();
	//if players health is less than 0
	if (CurrentHealth <= 0)
		OnPlayerDeath();

	return DamageAmount;
}
void APlanePawn::UpdateHealthAndShield() 
{
	if(widgetHUDInstance)
	{
		widgetHUDInstance->UpdateHealth(CurrentHealth,MaxHealth);
		widgetHUDInstance->UpdateShield(CurrentShield,MaxShield);
	}
}
void APlanePawn::OnPlayerDeath()
{
	auto PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PC->SetPause(true);
	PC->bShowMouseCursor = true;
	PC->bEnableClickEvents = true;
	PC->bEnableMouseOverEvents = true;
	if (widgetDeathScreenInstance)
		widgetDeathScreenInstance->AddToPlayerScreen();
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
		UpdateHealthAndShield();
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
	for (UHardpoint* a : hardpoints) 
	{
		if (a->thisShootButton == ShootButton::DOWN)
			a->IsShooting=true;
	}
}
void APlanePawn::ProcessFire1Released()
{
	{
		for (UHardpoint* a : hardpoints)
		{
			if (a->thisShootButton == ShootButton::DOWN)
				a->IsShooting = false;
		}
	}
}
void APlanePawn::ProcessFire2Pressed()
{
	for (UHardpoint* a : hardpoints)
	{
		if (a->thisShootButton == ShootButton::LEFT)
			a->IsShooting = true;
	}
}
void APlanePawn::ProcessFire2Released()
{
	for (UHardpoint* a : hardpoints)
	{
		if (a->thisShootButton == ShootButton::LEFT)
			a->IsShooting = false;
	}
}
void APlanePawn::ProcessFire3Pressed()
{
	for (UHardpoint* a : hardpoints)
	{
		if (a->thisShootButton == ShootButton::UP)
			a->IsShooting = true;
	}
}
void APlanePawn::ProcessFire3Released()
{
	for (UHardpoint* a : hardpoints)
	{
		if (a->thisShootButton == ShootButton::UP)
			a->IsShooting = false;
	}
}
void APlanePawn::ProcessEvadePressed()
{
	IsAOAOn = true;
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
