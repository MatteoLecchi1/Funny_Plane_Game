// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretPawn.h"
#include "PlaneGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
ATurretPawn::ATurretPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATurretPawn::BeginPlay()
{

	FString TheFloatStr = FString::FromInt(Cast<APlaneGameMode>(GetWorld()->GetAuthGameMode())->CurrentObjective);
	Super::BeginPlay();

	GetComponents<UHardpoint>(Hardpoints, true);

	turretBase = Cast<USceneComponent>(GetComponentsByTag(USceneComponent::StaticClass(), "turretBase")[0]);
	turretGimball = Cast<USceneComponent>(GetComponentsByTag(USceneComponent::StaticClass(), "turretGimball")[0]);

	TheFloatStr = FString::FromInt(Cast<APlaneGameMode>(GetWorld()->GetAuthGameMode())->CurrentObjective);
	//add this actor to gamemode lists
	auto gamemode = Cast<APlaneGameMode>(GetWorld()->GetAuthGameMode());
	if (gamemode)
	{
		gamemode->AddActorToArrays(this);
	}
	TheFloatStr = FString::FromInt(Cast<APlaneGameMode>(GetWorld()->GetAuthGameMode())->CurrentObjective);

	CurrentHealth = MaxHealth;
}

// Called every frame
void ATurretPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//RotateBase
	turretBase->AddLocalRotation(FRotator(0, RotationSpeed * CurrentYaw * DeltaTime, 0));
	//RotateGimball
	turretGimball->AddLocalRotation(FRotator(RotationSpeed * CurrentPitch * DeltaTime, 0, 0));

	if (!FreeRotation)
	{
		//clamp Base rotation
		if (turretBase->GetRelativeRotation().Yaw > MaxYaw)
		{
			turretBase->SetRelativeRotation(FRotator(0, MaxYaw, 0));
		}
		else if (turretBase->GetRelativeRotation().Yaw < MinYaw)
		{
			turretBase->SetRelativeRotation(FRotator(0, MinYaw, 0));
		}
	}
	//clamp gimball rotation
	if (turretGimball->GetRelativeRotation().Pitch > MaxPitch)
	{
		turretGimball->SetRelativeRotation(FRotator(MaxPitch, 0, 0));
	}
	else if (turretGimball->GetRelativeRotation().Pitch < MinPitch)
	{
		turretGimball->SetRelativeRotation(FRotator(MinPitch, 0, 0));
	}
}

// Called to bind functionality to input
void ATurretPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
float ATurretPawn::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	CurrentHealth -= DamageAmount;

	//if turret health is less than 0
	if (CurrentHealth <= 0)
		MyDestroy();

	return DamageAmount;
}

void ATurretPawn::MyDestroy()
{
	Destroy();
	auto gamemode = Cast<APlaneGameMode>(GetWorld()->GetAuthGameMode());
	//remove this actor from gamemode lists
	if (gamemode)
	{
		gamemode->RemoveActorFromArrays(this);
	}
	if (ExplosionEffect) {
		UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), FRotator(0.f), FVector(1));
	}
}

