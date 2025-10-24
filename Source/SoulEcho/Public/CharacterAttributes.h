// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterAttributes.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOULECHO_API UCharacterAttributes : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterAttributes();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE float& GetMaxHealth() { return MaxHealth; }
	FORCEINLINE float& GetCurrentHealth() { return CurrentHealth; }
	FORCEINLINE float& GetMaxStamina() { return MaxStamina; }
	FORCEINLINE float& GetCurrentStamina() { return CurrentStamina; }
	FORCEINLINE int32& GetGold() { return Gold; }
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"), Category="CharacterAttributes")
	float MaxHealth;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"), Category="CharacterAttributes")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"), Category="CharacterAttributes")
	float MaxStamina;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"), Category="CharacterAttributes")
	float CurrentStamina;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"), Category="CharacterAttributes")
	int32 Gold;
};
