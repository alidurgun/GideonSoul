// Fill out your copyright notice in the Description page of Project Settings.


#include "GideonAnimInstance.h"
#include "GideonCharacter.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <Kismet/KismetMathLibrary.h>

void UGideonAnimInstance::NativeInitializeAnimation()
{
	GroundSpeed = 0.0f;
	IsFalling = false;
	Gideon = Cast<AGideonCharacter>(TryGetPawnOwner());
	
	if (Gideon) {
		GideonMovement = Gideon->GetCharacterMovement();
	}
}

void UGideonAnimInstance::NativeUpdateAnimation(float DeltaTimeX)
{
	if (Gideon && GideonMovement) {
		GroundSpeed = UKismetMathLibrary::VSizeXY(GideonMovement->Velocity);
		IsFalling = GideonMovement->IsFalling();
	}
}

void UGideonAnimInstance::SetCombatStateCharacter(ECombatStates NewCombatState)
{
	if (Gideon)
	{
		Gideon->SetCombatState(NewCombatState);
	}
}
