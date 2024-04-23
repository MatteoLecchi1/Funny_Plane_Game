// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/RailgunHardpointWeapon.h"
#include "PlaneGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

void URailgunHardpointWeapon::Shoot(AActor* PossibleTarget)
{

	//spawn projectile and assign
	FTransform SpawnTransform = GetSocketTransform("ProjectileSpawnLocation1", ERelativeTransformSpace::RTS_Component);
	SpawnTransform = SpawnTransform * GetComponentTransform();

	FVector TraceStart = SpawnTransform.GetLocation();
	FVector TraceEnd = TraceStart + GetForwardVector() * range;

	//add ignored actors
	FCollisionQueryParams QueryParams;

	QueryParams.AddIgnoredActor(GetOwner());

	auto gamemode = Cast<APlaneGameMode>(GetWorld()->GetAuthGameMode());

	TArray<AActor*> TemporaryArray;
	if (GetOwner()->ActorHasTag("IsFriendly")) 
	{
		QueryParams.AddIgnoredActors(gamemode->FriendlyActors); 
		TemporaryArray = gamemode->FriendlyActors;
		TemporaryArray.Add(gamemode->PlayerActor);
	}
	else if (GetOwner()->ActorHasTag("IsEnemy")) 
	{
		QueryParams.AddIgnoredActors(gamemode->EnemyActors); TemporaryArray = gamemode->EnemyActors;
	}

	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, TraceChannelProperty, QueryParams);
	if (Hit.bBlockingHit)
	{
		UGameplayStatics::ApplyDamage(Hit.GetActor(), DamageOverride, UGameplayStatics::GetPlayerController(GetWorld(), 0), GetOwner(), nullptr);

		UGameplayStatics::ApplyRadialDamage(GetWorld(), DamageOverride, Hit.Location, AreaDamageRadiusOverride, nullptr, TemporaryArray, GetOwner(), UGameplayStatics::GetPlayerController(GetWorld(), 0), true);

		if (ExplosionEffect)
		{
			UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionEffect, Hit.Location, FRotator(0.f), FVector(AreaDamageRadiusOverride));
		}
	}
	if (TrailEffect)
	{
		UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TrailEffect, SpawnTransform.GetLocation(), FRotator(0.f));
		if(Hit.bBlockingHit)
		{
			NiagaraComp->SetNiagaraVariableVec3("BeamEndLocation", Hit.Location);
		}
		else
		{
			NiagaraComp->SetNiagaraVariableVec3("BeamEndLocation", TraceStart + GetForwardVector() * range);
		}
	}
}