// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
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
	float ProjectileLifespan=5; 
	UPROPERTY(EditAnywhere)
	float DamageDealt = 10;
	UPROPERTY(EditAnywhere)
	float AreaDamageRadius = 100;
	

	UPROPERTY(EditAnywhere, Category = "VFX")
	UNiagaraSystem* ExplosionEffect;


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
