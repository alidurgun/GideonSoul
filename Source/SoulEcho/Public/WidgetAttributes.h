// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "WidgetAttributes.generated.h"

class UHealthBarUW;

/**
 * 
 */
UCLASS()
class SOULECHO_API UWidgetAttributes : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UHealthBarUW* HealthBarUw;

	void SetHealthPercentage(float percentage);
};
