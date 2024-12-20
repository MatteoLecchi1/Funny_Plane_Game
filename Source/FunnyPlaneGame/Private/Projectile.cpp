// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "PlaneGameMode.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh -> SetSimulatePhysics(true);
	RootComponent = ProjectileMesh;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	ProjectileMesh->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);
	this->SetLifeSpan(ProjectileLifespan);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
void AProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->StaticClass() != TSubclassOf<AProjectile>()){
	if (ActorHasTag("IsFriendlyProjectile")) 
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
	else if(ActorHasTag("IsEnemyProjectile"))
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
}
void AProjectile::DestroySelf()
{
	auto gamemode = Cast<APlaneGameMode>(GetWorld()->GetAuthGameMode());
	if (ActorHasTag("IsFriendlyProjectile")) {
		//this array contains all "FriendlyActors", the players pawn and self
		TArray<AActor*> TemporaryArray = gamemode->FriendlyActors;
		TemporaryArray.Add(this);
		TemporaryArray.Add(gamemode->PlayerActor);
		UGameplayStatics::ApplyRadialDamage(GetWorld(), DamageDealt, GetActorLocation(), AreaDamageRadius, nullptr, TemporaryArray, this, UGameplayStatics::GetPlayerController(GetWorld(), 0), true, ECollisionChannel::ECC_GameTraceChannel2);
	}
	else if (ActorHasTag("IsEnemyProjectile")) {
		//this array contains all "EnemyActors" and self
		TArray<AActor*> TemporaryArray = gamemode->EnemyActors;
		TemporaryArray.Add(this);
		UGameplayStatics::ApplyRadialDamage(GetWorld(), DamageDealt, GetActorLocation(), AreaDamageRadius, nullptr, TemporaryArray, this, UGameplayStatics::GetPlayerController(GetWorld(), 0), true, ECollisionChannel::ECC_GameTraceChannel2);
	}
	if (ExplosionEffect) {
		UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), FRotator(0.f),FVector(AreaDamageRadius));
	}
	Destroy();
}

