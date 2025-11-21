// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include <Enemy.h>

#include "GameFramework/CharacterMovementComponent.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	ActorState = EActorState::EAS_Alive;
	Enemy = Cast<AEnemy>(TryGetPawnOwner());
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (Enemy)
	{
		ActorState = Enemy->GetActorState();
		EnemyGroundSpeed = Enemy->GetCharacterMovement()->Velocity.Size();
	}
}

void UEnemyAnimInstance::ChangeEnemyCombatState(ECombatStates newState)
{
	if (Enemy)
	{
		Enemy->SetEnemyCombatState(newState);
	}
}
