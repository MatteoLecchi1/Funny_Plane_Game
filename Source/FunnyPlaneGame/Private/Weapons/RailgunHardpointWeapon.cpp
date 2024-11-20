// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/RailgunHardpointWeapon.h"
#include "PlaneGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

void URailgunHardpointWeapon::Shoot(AActor* PossibleTarget)
{
	Super::Shoot(PossibleTarget);

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
		QueryParams.AddIgnoredActors(gamemode->EnemyActors);
		TemporaryArray = gamemode->EnemyActors;
	}
	if (AActor* OwnerOfOwner = GetOwner()->GetOwner()) 
	{
		TemporaryArray.Add(OwnerOfOwner);
	}
	SpawnBarrelEffect(SpawnTransform);

	FHitResult ShpereHit;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(TraceChannelProperty));

	UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), TraceStart, TraceEnd, Radius, ObjectTypesArray, false, TemporaryArray, EDrawDebugTrace::ForDuration, ShpereHit, true);
	if (ShpereHit.bBlockingHit)
	{
		UGameplayStatics::ApplyDamage(ShpereHit.GetActor(), DamageOverride, UGameplayStatics::GetPlayerController(GetWorld(), 0), GetOwner(), nullptr);
		UGameplayStatics::ApplyRadialDamage(GetWorld(), DamageOverride, ShpereHit.Location, AreaDamageRadiusOverride, nullptr, TemporaryArray, GetOwner(), UGameplayStatics::GetPlayerController(GetWorld(), 0), true, ECollisionChannel::ECC_GameTraceChannel2);

		if (ExplosionEffect)
		{
			UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionEffect, ShpereHit.Location, FRotator(0.f), FVector(AreaDamageRadiusOverride));
		}
		if (TrailEffect) 
		{
			SpawnTrailEffect(SpawnTransform.GetLocation(), ShpereHit.Location);
		}
		return;
	}

	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, TraceChannelProperty, QueryParams);

	if(Hit.bBlockingHit)
	{
		UGameplayStatics::ApplyDamage(Hit.GetActor(), DamageOverride, UGameplayStatics::GetPlayerController(GetWorld(), 0), GetOwner(), nullptr);
		UGameplayStatics::ApplyRadialDamage(GetWorld(), DamageOverride, Hit.Location, AreaDamageRadiusOverride, nullptr, TemporaryArray, GetOwner(), UGameplayStatics::GetPlayerController(GetWorld(), 0), true, ECollisionChannel::ECC_GameTraceChannel2);

		if (ExplosionEffect)
		{
			UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionEffect, Hit.Location, FRotator(0.f), FVector(AreaDamageRadiusOverride));
		}
		if (TrailEffect)
		{
			SpawnTrailEffect(SpawnTransform.GetLocation(), Hit.Location);
		}
		return;
	}

	if (TrailEffect) 
	{
		SpawnTrailEffect(SpawnTransform.GetLocation(), TraceStart + GetForwardVector() * range);
	}
}
void URailgunHardpointWeapon::SpawnTrailEffect(FVector start, FVector End)
{
	UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TrailEffect, start, FRotator(0.f));
	NiagaraComp->SetVectorParameter("BeamEndLocation", End);
}