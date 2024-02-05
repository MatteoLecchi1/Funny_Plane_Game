// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "CombatManager.h"
#include "Projectile.generated.h"


UCLASS()
class FUNNYPLANEGAME_API AProjectile : public AActor
{
	GENERATED_BODY()
	

public:	
	// Sets default values for this actor's properties
	AProjectile();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* ProjectileComponent;

	UPROPERTY()
	ACombatManager* CombatManager;

	UPROPERTY(EditAnywhere)
	float ProjectileLifespan=10; 
	UPROPERTY(EditAnywhere)
	float DamageDealt = 10;
	UPROPERTY(EditAnywhere)
	float AreaDamageRadius = 100;
	


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void DestroySelf();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
