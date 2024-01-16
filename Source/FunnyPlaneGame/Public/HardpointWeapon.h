// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.h"
#include "HardpointWeapon.generated.h"

UCLASS()
class FUNNYPLANEGAME_API AHardpointWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHardpointWeapon();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* gunMesh;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> projectile;

	UPROPERTY(EditAnywhere)
	float fireRate;
	float fireDelay;
	bool shouldFire;

	UPROPERTY(EditAnywhere,Meta = (MakeEditWidget = true))
	FVector projectileSpawnLocation;
		 
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
