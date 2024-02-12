// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "HardpointWeapon.generated.h"

UCLASS()
class FUNNYPLANEGAME_API UHardpointWeapon : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UHardpointWeapon();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> projectile;

	UPROPERTY(BlueprintReadWrite)
	AActor* PlaneOwner;

	UPROPERTY(EditAnywhere)
	float DamageOverride = -1;
	UPROPERTY(EditAnywhere)
	float AreaDamageRadiusOverride = -1;

	UPROPERTY(EditAnywhere)
	float fireSpeed = 10000;
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
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shooting")
	void Shoot();
};
