// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarUW.generated.h"

class UProgressBar;

/**
 * 
 */
UCLASS()
class SOULECHO_API UHealthBarUW : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UProgressBar* HealthBar;
};
