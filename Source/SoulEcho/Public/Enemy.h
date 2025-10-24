// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitInterface.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class UCharacterAttributes;

UCLASS()
class SOULECHO_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	const float CapsuleHalfHeight{ 89.5f };
	const float CapsuleRadius{ 34.0f };

	UPROPERTY()
	UCharacterAttributes* Attributes;
	
	const float DefaultAttributeValue { 100.0f };
};
