// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include <Components/BoxComponent.h>

#include "HitInterface.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"

AWeapon::AWeapon()
{
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBox"));
	WeaponBox->SetGenerateOverlapEvents(false);
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponBox->SetupAttachment(GetRootComponent());

	WeaponStart = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponStart"));
	WeaponEnd = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponEnd"));
	WeaponStart->SetupAttachment(GetRootComponent());
	WeaponEnd->SetupAttachment(GetRootComponent());
}

void AWeapon::WeaponBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector start = WeaponStart->GetComponentLocation();
	const FVector end = WeaponEnd->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	FHitResult Hit;
	
	UKismetSystemLibrary::BoxTraceSingle(this, start, end, FVector(30.0f,30.0f,30.0f),
		WeaponStart->GetComponentRotation(),TraceTypeQuery1, false, ActorsToIgnore,
		EDrawDebugTrace::ForDuration,Hit,true);

	UE_LOG(LogTemp, Error, TEXT("WeaponBox Overlap!"));
	
	if (IHitInterface* HitInterface = Cast<IHitInterface>(Hit.GetActor()))
	{
		HitInterface->GetHit(Hit.ImpactPoint);
	}
	
}

void AWeapon::Equip(USceneComponent* SceneComponent, const FName SocketName) const
{
	const FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::SnapToTarget,EAttachmentRule::SnapToTarget,false);
	ItemMesh->AttachToComponent(SceneComponent, Rules,SocketName);
}


void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::WeaponBoxOverlap);
}

