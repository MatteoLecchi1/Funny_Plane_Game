// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Hardpoint.generated.h"

UENUM(BlueprintType)
enum class ShootButton : uint8
{
	DOWN = 0	UMETA(DisplayName = "DOWN"),
	LEFT = 1	UMETA(DisplayName = "LEFT"),
	UP = 2		UMETA(DisplayName = "UP"),
	RIGHT = 3	UMETA(DisplayName = "RIGHT")
};
UENUM(BlueprintType)
enum class HardpointTier : uint8
{
	TIER0 = 0	UMETA(DisplayName = "TIER0"),
	TIER1 = 1	UMETA(DisplayName = "TIER1"),
	TIER2 = 2	UMETA(DisplayName = "TIER2"),
	TIER3 = 3	UMETA(DisplayName = "TIER3")
};
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FUNNYPLANEGAME_API UHardpoint : public UStaticMeshComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHardpoint();

	UPROPERTY(EditDefaultsOnly)
	FName DefaultHardpointWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ShootButton thisShootButton = ShootButton::RIGHT;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	HardpointTier thisHardpointTier = HardpointTier::TIER0;
	UPROPERTY(BlueprintReadOnly)
	class UHardpointWeapon* WeaponInstance;
	UPROPERTY()
	class AActor* CurrentTarget;

	UFUNCTION()
	void ShootWeapon(AActor* PossibleTarget);
	UFUNCTION()
	void AssignWeapon(const FName& WeaponKey);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};