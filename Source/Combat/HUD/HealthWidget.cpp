

#include "HealthWidget.h"
#include "Components/ProgressBar.h"

void UHealthWidget::UpdateHealth_HUD(const float& NewHealth, const float& MaxHealth)
{
	if (HealthBar)
	{
		float Percent = NewHealth / MaxHealth;
		HealthBar->SetPercent(Percent);
	}
}
