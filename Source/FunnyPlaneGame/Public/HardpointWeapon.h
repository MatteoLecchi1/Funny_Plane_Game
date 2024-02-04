// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

	UPROPERTY(BlueprintReadWrite)
	AActor* PlaneOwner;

	UPROPERTY(EditAnywhere)
	float DamageOverride = -1;

	UPROPERTY(EditAnywhere)
	float fireRate = 10;
	UPROPERTY(EditAnywhere)
	float fireSpread = 0;
	float fireDelay = 0;
	bool shouldFire = false;

	UPROPERTY()
	FVector projectileSpawnLocation;
	FRandomStream RandomStream;
		 
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shooting")
	void Shoot();
};
