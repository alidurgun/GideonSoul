// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <CharacterStates.h>
#include "GideonCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class SOULECHO_API AGideonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGideonCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Jump() override;

	FORCEINLINE void SetOverlappedMesh(UStaticMeshComponent* overlapMesh){ this->OverlappedMesh = overlapMesh; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	float CapsuleHalfHeight{ 89.5f };
	float CapsuleRadius{ 34.0f };

	void GoForwardBackward(float Scale);
	void GoLeftRight(float Scale);
	void LookLeftRight(float Scale);
	void LookUpDown(float Scale);
	void TakeWeapon();
	void EquipWeapon(UStaticMeshComponent* MeshComponent, FName SocketName);
	void ArmDisarm();

	UPROPERTY()
	USpringArmComponent* ArmComponent;

	UPROPERTY()
	UCameraComponent* CameraComponent;

	float DefaultArmLenght{ 400.0f };

	UPROPERTY()
	UStaticMeshComponent* OverlappedMesh;

	UPROPERTY()
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TEnumAsByte<ECharacterStates> CharacterState { ECharacterStates::ECS_Unequipped };
};
