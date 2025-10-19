// Fill out your copyright notice in the Description page of Project Settings.

#include "GideonCharacter.h"
#include <Components/CapsuleComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>

#include "Chaos/AABBTree.h"

// Sets default values
AGideonCharacter::AGideonCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Then our character will turn it's face to whereever we go.
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator{ 0.0f,400.0f,0.0f };

	GetCapsuleComponent()->SetCapsuleHalfHeight(CapsuleHalfHeight);
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetCapsuleComponent()->SetCapsuleRadius(CapsuleRadius);

	SetRootComponent(GetCapsuleComponent());

	ArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Arm"));
	ArmComponent->TargetArmLength = DefaultArmLenght;
	ArmComponent->bUsePawnControlRotation = true; //This should be set to true in order to rotate camera.
	ArmComponent->SetupAttachment(GetRootComponent());

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(ArmComponent);

	AttackNames.Add(FName("Attack1"));
	AttackNames.Add(FName("Attack2"));
	AttackNames.Add(FName("Attack3"));
}

// Called when the game starts or when spawned
void AGideonCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AGideonCharacter::GoForwardBackward(float Scale)
{
	if (CanMove(Scale))
	{
		const FRotator YawRotation{ 0.0f, GetControlRotation().Yaw, 0.0f };
		const FVector Direction{ FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X) };
		AddMovementInput(Direction, Scale);
	}
}

void AGideonCharacter::GoLeftRight(float Scale)
{
	if (CanMove(Scale))
	{
		const FRotator YawRotation{ 0.0f, GetControlRotation().Yaw, 0.0f };
		const FVector Direction{ FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y) };
		AddMovementInput(Direction, Scale);
	}
}

void AGideonCharacter::LookLeftRight(float Scale)
{
	if (Scale != 0.0f && GetController()) {
		AddControllerYawInput(Scale);
	}
}

void AGideonCharacter::LookUpDown(float Scale)
{
	if (Scale != 0.0f && GetController()) {
		AddControllerPitchInput(Scale);
	}
}

void AGideonCharacter::TakeWeapon()
{
	if (OverlappedMesh && GetMesh() && CharacterState == ECharacterStates::ECS_Unequipped)
	{
		EquipWeapon(OverlappedMesh, FName("WeaponSocket"));
		WeaponMesh = OverlappedMesh;
		CharacterState = ECharacterStates::ECS_1HEquipped;
	}
}

void AGideonCharacter::EquipWeapon(UStaticMeshComponent* MeshComponent, FName SocketName) const
{
	const FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			false);
		
	MeshComponent->AttachToComponent(GetMesh(), Rules, SocketName);
}

void AGideonCharacter::ArmDisarm()
{
	if (CharacterState == ECharacterStates::ECS_Disarm && WeaponMesh)
	{
		EquipWeapon(WeaponMesh, FName("WeaponSocket"));
		CharacterState = ECharacterStates::ECS_1HEquipped;
	}
	else
	{
		EquipWeapon(WeaponMesh, FName("DisarmSocket"));
		CharacterState = ECharacterStates::ECS_Disarm;
	}
}

void AGideonCharacter::Attack()
{
	if (CanAttack())
	{
		// play montage
		int32 index = FMath::RandRange(0,AttackNames.Num() - 1);
		UAnimInstance* anim {GetMesh()->GetAnimInstance()};
		if (anim && AttackMontage)
		{
			anim->Montage_Play(AttackMontage);
			anim->Montage_JumpToSection(AttackNames[index], AttackMontage);

			CombatState = ECombatStates::ECS_Attacking;
		}
	}
}

bool AGideonCharacter::CanMove(float Scale)
{
	return GetController() && Scale != 0.0f && CombatState != ECombatStates::ECS_Attacking;
}

bool AGideonCharacter::CanAttack()
{
	return CombatState != ECombatStates::ECS_Attacking && !AttackNames.IsEmpty() && (CharacterState == ECharacterStates::ECS_1HEquipped || CharacterState == ECharacterStates::ECS_2HEquipped);
}

// Called every frame
void AGideonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AGideonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("Forward"), this, &AGideonCharacter::GoForwardBackward);
	PlayerInputComponent->BindAxis(FName("Backward"), this, &AGideonCharacter::GoForwardBackward);
	PlayerInputComponent->BindAxis(FName("TurnRight"), this, &AGideonCharacter::GoLeftRight);
	PlayerInputComponent->BindAxis(FName("TurnLeft"), this, &AGideonCharacter::GoLeftRight);

	PlayerInputComponent->BindAxis(FName("LookUpDown"), this, &AGideonCharacter::LookUpDown);
	PlayerInputComponent->BindAxis(FName("LookRightLeft"), this, &AGideonCharacter::LookLeftRight);

	PlayerInputComponent->BindAction(FName("Jump"), EInputEvent::IE_Pressed, this, &AGideonCharacter::Jump);

	PlayerInputComponent->BindAction(FName("EquipWeapon"),EInputEvent::IE_Pressed, this, &AGideonCharacter::TakeWeapon);
	PlayerInputComponent->BindAction(FName("ArmDisarm"), IE_Pressed, this, &AGideonCharacter::ArmDisarm);

	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &AGideonCharacter::Attack);
}

void AGideonCharacter::Jump()
{
	Super::Jump();
}