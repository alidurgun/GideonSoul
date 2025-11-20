// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include <GideonCharacter.h>

#include "HitInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ASword::ASword()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordMesh"));
	SwordMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	RootComponent = SwordMesh;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetGenerateOverlapEvents(true);
	Sphere->SetCollisionResponseToAllChannels(ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	Sphere->SetupAttachment(GetRootComponent());

	SwordBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBox"));
	SwordBox->SetGenerateOverlapEvents(false);
	SwordBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	SwordBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SwordBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	SwordBox->SetupAttachment(GetRootComponent());

	SwordBoxStart = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponStart"));
	SwordBoxEnd = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponEnd"));
	SwordBoxStart->SetupAttachment(GetRootComponent());
	SwordBoxEnd->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ASword::BeginPlay()
{
	Super::BeginPlay();

	SwordBox->OnComponentBeginOverlap.AddDynamic(this, &ASword::SwordBoxOverlap);
	SwordBox->OnComponentEndOverlap.AddDynamic(this, &ASword::SwordBoxEndOverlap);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ASword::SwordSphereBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ASword::SwordSphereEndOverlap);
}

// Called every frame
void ASword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASword::Equip(USceneComponent* SceneComponent, const FName SocketName, AController* Holder)
{
	const FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
	                                                                  EAttachmentRule::SnapToTarget,
	                                                                  EAttachmentRule::SnapToTarget, false);
	SwordMesh->AttachToComponent(SceneComponent, Rules, SocketName);
	if (Sphere)
	{
		IsEquipped = true;
		Sphere->SetCollisionEnabled((ECollisionEnabled::Type::NoCollision));
		Sphere->SetGenerateOverlapEvents(false);
	}

	HitController = Holder;
}

void ASword::SwordBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                             const FHitResult& SweepResult)
{
	const FVector start = SwordBoxStart->GetComponentLocation();
	const FVector end = SwordBoxEnd->GetComponentLocation();
	
	FHitResult Hit;
	
	UKismetSystemLibrary::BoxTraceSingle(this, start, end, FVector(30.0f,30.0f,30.0f),
		SwordBoxStart->GetComponentRotation(),TraceTypeQuery1, false, ActorsToIgnore,
		EDrawDebugTrace::None,Hit,true);

	UE_LOG(LogTemp, Error, TEXT("WeaponBox Overlap!"));
	
	if (IHitInterface* HitInterface = Cast<IHitInterface>(Hit.GetActor()))
	{
		HitInterface->Execute_GetHit(Hit.GetActor(), Hit.ImpactPoint);
		UGameplayStatics::ApplyDamage(Hit.GetActor(),20,HitController,this,UDamageType::StaticClass());
		ActorsToIgnore.AddUnique(Hit.GetActor());
	}
}

void ASword::SwordBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
								   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ActorsToIgnore.Empty();
}

void ASword::SwordSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                     const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Display, TEXT("ItemSphereBeginOverlap!"));
	if (AGideonCharacter* GideonCharacter = Cast<AGideonCharacter>(OtherActor))
	{
		GideonCharacter->SetWeaponClass(this);
	}
}

void ASword::SwordSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsEquipped) return;
	//UE_LOG(LogTemp, Display, TEXT("ItemSphereBeginOverlap!"));
	if (AGideonCharacter* GideonCharacter = Cast<AGideonCharacter>(OtherActor))
	{
		GideonCharacter->SetWeaponClass(nullptr);
	}
}
