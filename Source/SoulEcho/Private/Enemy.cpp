// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "CharacterAttributes.h"
#include "WidgetAttributes.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCapsuleHalfHeight(CapsuleHalfHeight);
	GetCapsuleComponent()->SetCapsuleRadius(CapsuleRadius);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	Attributes = CreateDefaultSubobject<UCharacterAttributes>(TEXT("Attributes"));
	
	WidgetAttributes = CreateDefaultSubobject<UWidgetAttributes>(TEXT("WidgetAttributes"));
	WidgetAttributes->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (Attributes)
	{
		Attributes->GetMaxHealth() = DefaultAttributeValue;
		Attributes->GetCurrentHealth() = DefaultAttributeValue;
		Attributes->GetMaxStamina() = DefaultAttributeValue;
		Attributes->GetCurrentStamina() = DefaultAttributeValue;
		if (WidgetAttributes)
		{
			WidgetAttributes->SetVisibility(true);
			WidgetAttributes->SetHealthPercentage(Attributes->GetCurrentHealth() / Attributes->GetMaxHealth());
		}
	}
}

void AEnemy::Die()
{
	UAnimInstance* anim = GetMesh()->GetAnimInstance();
	if (anim && DeathMontage)
	{
		anim->Montage_Play(DeathMontage);
		anim->Montage_JumpToSection(FName("Death"), DeathMontage);
		ActorState = EActorState::EAS_Dead;
	}
}

void AEnemy::StartPatrol()
{
	EnemyMutex.lock();
	this->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	EnemyMutex.unlock();
}

void AEnemy::ChaseEnemy()
{
	EnemyMutex.lock();
	this->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	EnemyMutex.unlock();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	UE_LOG(LogTemp,Warning,TEXT("GetHit Called from Enemy."));

}

float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
	AActor* DamageCauser)
{
	if (Attributes && WidgetAttributes)
	{
		float &currentHealth = Attributes->GetCurrentHealth();
		currentHealth = FMath::Clamp(currentHealth - DamageAmount, 0.0f, Attributes->GetMaxHealth());
		
		UE_LOG(LogTemp,Display,TEXT("CurrentHealth = %f"),currentHealth);
		WidgetAttributes->SetHealthPercentage(currentHealth / Attributes->GetCurrentHealth());
		
		if (currentHealth <= 0.0f)
		{
			Die();
			SetLifeSpan(5.0f);
		}
		else
		{
			// animation.
			UAnimInstance* anim = GetMesh()->GetAnimInstance();
			if (anim && HitReactMontage)
			{
				anim->Montage_Play(HitReactMontage);
				anim->Montage_JumpToSection(FName("GetHit"), HitReactMontage);
				EnemyState = ECombatStates::ECS_GetHit;
			}
		}
	}

	return DamageAmount;
}

