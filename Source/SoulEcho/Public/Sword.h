// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sword.generated.h"

class USphereComponent;
class UBoxComponent;

UCLASS()
class SOULECHO_API ASword : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASword();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Equip(USceneComponent* SceneComponent, const FName SocketName);

	UFUNCTION()
	virtual void SwordBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void SwordSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void SwordSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	FORCEINLINE UBoxComponent* GetSwordBox() const { return SwordBox; }

	FORCEINLINE UStaticMeshComponent* GetSwordMesh() const { return SwordMesh; }
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Visualization")
	UStaticMeshComponent* SwordMesh;

	UPROPERTY(EditAnywhere, Category = "Components")
	USphereComponent* Sphere;
	
	UPROPERTY(EditAnywhere, Category = "Components")
	UBoxComponent* SwordBox;

	UPROPERTY(EditAnywhere, Category = "Components")
	USceneComponent* SwordBoxStart;

	UPROPERTY(EditAnywhere, Category = "Components")
	USceneComponent* SwordBoxEnd;
	
	bool IsEquipped {false};
};
