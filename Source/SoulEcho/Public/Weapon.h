// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

class UBoxComponent;

/**
 * 
 */
UCLASS()
class SOULECHO_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();

	void WeaponBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// This is responsible of collision along the sword itself. This box component will cover all blade itself.
	UPROPERTY(EditAnywhere)
	UBoxComponent* WeaponBox;

	UPROPERTY(EditAnywhere)
	USceneComponent* WeaponStart;

	UPROPERTY(EditAnywhere)
	USceneComponent* WeaponEnd;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
