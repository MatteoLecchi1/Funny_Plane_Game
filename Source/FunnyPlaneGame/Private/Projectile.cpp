// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Kismet/GameplayStatics.h"

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
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ProjectileLifespan -= DeltaTime;
	if (ProjectileLifespan < 0)
		DestroySelf();
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
			if (OtherActor->CanBeDamaged())
			{
				UGameplayStatics::ApplyDamage(OtherActor, DamageDealt, UGameplayStatics::GetPlayerController(GetWorld(), 0), this, nullptr);
			}
			DestroySelf();
		}
		else 
		{   //hits others
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
		{   //hits others
			DestroySelf();
		}
	}
}
void AProjectile::DestroySelf()
{
	Destroy();
}

