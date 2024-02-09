// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "CombatManager.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->SetSimulatePhysics(true);

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	ProjectileMesh->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);
	CombatManager = Cast<ACombatManager>(UGameplayStatics::GetActorOfClass(GetWorld(),ACombatManager::StaticClass()));
	this->SetLifeSpan(ProjectileLifespan);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ActorHasTag("IsFriendly")) 
	{
		if (OtherActor->ActorHasTag("IsFriendly"))
		{	//friendly hits friendly

		}
		else if (OtherActor->ActorHasTag("IsEnemy"))
		{   //friendly hits enemy
			UGameplayStatics::ApplyDamage(OtherActor, DamageDealt, UGameplayStatics::GetPlayerController(GetWorld(), 0), this, nullptr);
			DestroySelf();
		}
		else 
		{   //enemy hits others
			DestroySelf();
		}
	}
	else if(ActorHasTag("IsEnemy"))
	{
		if (OtherActor->ActorHasTag("IsFriendly"))
		{	//enemy hits friendly
			UGameplayStatics::ApplyDamage(OtherActor, DamageDealt, UGameplayStatics::GetPlayerController(GetWorld(), 0), this, nullptr);
			DestroySelf();
		}
		else if (OtherActor->ActorHasTag("IsEnemy"))
		{   //enemy hits enemy

		}
		else
		{   //enemy hits others
			DestroySelf();
		}
	}
}
void AProjectile::DestroySelf()
{
	if (CombatManager) {
		if (ActorHasTag("IsFriendly")) {
			//this array contains all "FriendlyActors" and self
			TArray<AActor*> TemporaryArray = CombatManager->FriendlyActors;
			TemporaryArray.Add(this);
			UGameplayStatics::ApplyRadialDamage(GetWorld(), DamageDealt, GetActorLocation(), AreaDamageRadius, nullptr, TemporaryArray, this, UGameplayStatics::GetPlayerController(GetWorld(), 0), true);
		}
		else if (ActorHasTag("IsEnemy")) {
			TArray<AActor*> TemporaryArray = CombatManager->EnemyActors;
			TemporaryArray.Add(this);
			UGameplayStatics::ApplyRadialDamage(GetWorld(), DamageDealt, GetActorLocation(), AreaDamageRadius, nullptr, TemporaryArray, this, UGameplayStatics::GetPlayerController(GetWorld(), 0), true);
		}
		if (ExplosionEffect) {
			UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), FRotator(0.f),FVector(AreaDamageRadius));
		}
	}
	Destroy();
}

