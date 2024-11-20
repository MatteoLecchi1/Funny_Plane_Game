// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/BossPawn.h"
#include "PlaneGameMode.h"

// Sets default values
ABossPawn::ABossPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABossPawn::BeginPlay()
{
	Super::BeginPlay();
	auto gamemode = Cast<APlaneGameMode>(GetWorld()->GetAuthGameMode());
	gamemode->Bosses.Add(this);
}

// Called every frame
void ABossPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABossPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
void ABossPawn::ChangePhase(int CurrentObjective)
{

}

