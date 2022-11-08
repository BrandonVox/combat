// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UCombatWidget::UpdateHealth_HUD(const float& NewHealth, const float& MaxHealth)
{
	if (HealthBar)
	{
		float Percent = NewHealth / MaxHealth;
		HealthBar->SetPercent(Percent);
	}

	if (HealthText && MaxHealthText)
	{
		// float -> int
		// char -> cstring -> Fstring 
		NewText = FString::Printf(TEXT("%d"), FMath::CeilToInt(NewHealth));
		HealthText->SetText(FText::FromString(NewText));

		NewText = FString::Printf(TEXT("%d"), FMath::CeilToInt(MaxHealth));
		MaxHealthText->SetText(FText::FromString(NewText));
	}

}

void UCombatWidget::UpdateEnergy_HUD(const float& NewEnergy, const float& MaxEnergy)
{
	if (EnergyBar)
	{
		float Percent = NewEnergy / MaxEnergy;
		EnergyBar->SetPercent(Percent);
	}

	if (EnergyText && MaxEnergyText)
	{
		// float -> int
		// char -> cstring -> Fstring 
		NewText = FString::Printf(TEXT("%d"), FMath::CeilToInt(NewEnergy));
		EnergyText->SetText(FText::FromString(NewText));

		NewText = FString::Printf(TEXT("%d"), FMath::CeilToInt(MaxEnergy));
		MaxEnergyText->SetText(FText::FromString(NewText));
	}
}
