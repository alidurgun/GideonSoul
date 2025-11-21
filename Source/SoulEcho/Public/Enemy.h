// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStates.h"
#include "HitInterface.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class UCharacterAttributes;
class UWidgetAttributes;
class UAnimMontage;
class AAIController;
class UPawnSensingComponent;

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

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, Category="Animations")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, Category="Animations")
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, Category="Animations")
	UAnimMontage* AttackMontage;

	// to fire patrol request with time.
	FTimerHandle PatrolTimer;

	// timer callback function.
	void PatrolTimerFinished();

	FORCEINLINE const EActorState GetActorState() const { return ActorState; }
	FORCEINLINE void SetEnemyCombatState(const ECombatStates newState) { EnemyState = newState; }

	// Callback function that will be called when the pawn has been seen.
	// For the Delegate functions we should tag it with UFUNCTION.
	UFUNCTION()
	void PawnSeen(APawn* Pawn);

	// Timer for attack.
	FTimerHandle AttackTimer;

	bool AttackTimerValid = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	const float CapsuleHalfHeight{ 89.5f };
	const float CapsuleRadius{ 34.0f };

	UPROPERTY()
	UCharacterAttributes* Attributes;

	UPROPERTY()
	UWidgetAttributes* WidgetAttributes;

	UPROPERTY()
	AAIController* AIController;

	UPROPERTY()
	UPawnSensingComponent* PawnSensingComponent;
	
	const float DefaultAttributeValue { 100.0f };

	const float WalkSpeed { 140.0f };
	const float RunSpeed { 350.0f };

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	ECombatStates EnemyState{ECombatStates::ECS_Free};
	
	EActorState ActorState{EActorState::EAS_Alive};

	void Die();

	void StartPatrol();

	void ChaseEnemy();

	UPROPERTY(EditAnywhere, Category="AI Movement", meta=(AllowPrivateAccess = "true"))
	TArray<AActor*> PatrolTargets;

	UPROPERTY(BlueprintReadOnly, Category="AI Movement", meta=(AllowPrivateAccess = "true"))
	AActor* CurrentPatrolTarget;

	UPROPERTY(BlueprintReadOnly, Category="Combat", meta=(AllowPrivateAccess = "true"))
	AActor* AttackTarget;

	const float AcceptanceRadius{ 120.0f };
	const float AttackRadius{400.0f}; // she is archer

	bool InTargetRange(float radius, AActor* target);

	void AttackToTarget();
};
