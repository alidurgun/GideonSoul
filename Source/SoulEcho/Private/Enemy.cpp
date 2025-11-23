// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "CharacterAttributes.h"
#include "WidgetAttributes.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"

#include "Perception/PawnSensingComponent.h"

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

	// enemy move related settings
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// PawnSensingComponent
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
	PawnSensingComponent->SightRadius = 1500.0f;
	PawnSensingComponent->SetPeripheralVisionAngle(45.0f);
}

void AEnemy::PatrolTimerFinished()
{
	if (EnemyState == ECombatStates::ECS_Free || EnemyState == ECombatStates::ECS_Patrolling)
		StartPatrol();
}

void AEnemy::PawnSeen(APawn* Pawn)
{
	if (Pawn->ActorHasTag("Player"))
		AttackTarget = Pawn;
}

FVector AEnemy::GetRotationWarpTarget()
{
	if (AttackTarget)
	{
		return AttackTarget->GetActorLocation();
	}
	
	return GetActorLocation();
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		GetWorldTimerManager().SetTimer(PatrolTimer,this,&AEnemy::PatrolTimerFinished,4.0f);
	}

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

	if (PawnSensingComponent)
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
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
	GetWorldTimerManager().SetTimer(PatrolTimer,this,&AEnemy::PatrolTimerFinished,4.0f);
	if (AttackTimer.IsValid())
	{
		GetWorldTimerManager().ClearTimer(AttackTimer);
		AttackTimerValid = false;
	}
	
	this->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	if (PatrolTargets.IsEmpty()) return;

	EnemyState = ECombatStates::ECS_Patrolling;
	
	int32 patrolIndex = FMath::RandRange(0, PatrolTargets.Num() - 1);
	CurrentPatrolTarget = PatrolTargets[patrolIndex];
	// moveto target.
	if (AIController && CurrentPatrolTarget)
	{
		FAIMoveRequest Request;
		Request.SetGoalActor(CurrentPatrolTarget);
		Request.SetAcceptanceRadius(20.0f);
		Request.SetCanStrafe(false); // Important for proper rotation
		Request.SetUsePathfinding(true);
		Request.SetAllowPartialPath(true);
		Request.SetRequireNavigableEndLocation(true);
		
		AIController->MoveTo(Request);
	}
}

void AEnemy::ChaseEnemy()
{
	CurrentPatrolTarget = nullptr;
	EnemyState = ECombatStates::ECS_Chasing;
	this->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	AIController->MoveToActor(AttackTarget);
}

bool AEnemy::InTargetRange(float radius, AActor* target)
{
	if (target == nullptr) return false;
	const float distance = (target->GetActorLocation()-GetActorLocation()).Size();
	return distance <= radius;
}

void AEnemy::AttackToTarget()
{
	UE_LOG(LogTemp, Warning, TEXT("Attacking to Target."));

	UAnimInstance* anim = GetMesh()->GetAnimInstance();	
	if (AttackMontage && anim)
	{
		anim->Montage_Play(AttackMontage);
		anim->Montage_JumpToSection(FName("Attack"), AttackMontage);
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentPatrolTarget && InTargetRange(AcceptanceRadius, CurrentPatrolTarget))
	{
		GetWorldTimerManager().SetTimer(PatrolTimer,this,&AEnemy::PatrolTimerFinished,4.0f);
	}
	if (EnemyState == ECombatStates::ECS_Attacking || EnemyState == ECombatStates::ECS_Chasing || EnemyState == ECombatStates::ECS_Engaged)
	{
		if (AttackTarget && !InTargetRange(AggroRadius, AttackTarget))
		{
			AttackTarget = nullptr;
			EnemyState = ECombatStates::ECS_Free;
			GetWorldTimerManager().SetTimer(PatrolTimer,this,&AEnemy::PatrolTimerFinished,1.0f);
		}
	}
	if (AttackTarget && InTargetRange(AggroRadius, AttackTarget))
	{
		if (AttackTarget && InTargetRange(AttackRadius, AttackTarget))
		{
			if (AIController)
				AIController->StopMovement();

			if (EnemyState != ECombatStates::ECS_Attacking)
			{
				EnemyState = ECombatStates::ECS_Attacking;
				GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::AttackToTarget, 3.5f);
			}
		}

		else if (AIController && AttackTarget && !InTargetRange(AttackRadius, AttackTarget) && EnemyState != ECombatStates::ECS_Chasing)
		{
			ChaseEnemy();
		}
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint, AActor* HitActor)
{
	UE_LOG(LogTemp,Warning,TEXT("GetHit Called from Enemy."));
	AttackTarget = HitActor;
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

