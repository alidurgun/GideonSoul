// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetAttributes.h"
#include <HealthBarUW.h>

#include "Components/ProgressBar.h"

void UWidgetAttributes::SetHealthPercentage(float percentage)
{
	if (HealthBarUw == nullptr)
	{
		HealthBarUw = Cast<UHealthBarUW>(GetUserWidgetObject());
	}

	if (HealthBarUw && HealthBarUw->HealthBar)
	{
		HealthBarUw->HealthBar->SetPercent(percentage);
	}
}
