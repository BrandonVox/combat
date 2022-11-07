// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatWidget.h"
#include "Components/ProgressBar.h"

void UCombatWidget::UpdateHealth_HUD(const float& NewHealth, const float& MaxHealth)
{
	if (HealthBar)
	{
		float Percent = NewHealth / MaxHealth;
		HealthBar->SetPercent(Percent);
	}

}

void UCombatWidget::UpdateEnergy_HUD(const float& NewEnergy, const float& MaxEnergy)
{
	if (EnergyBar)
	{
		float Percent = NewEnergy / MaxEnergy;
		EnergyBar->SetPercent(Percent);
	}
}
