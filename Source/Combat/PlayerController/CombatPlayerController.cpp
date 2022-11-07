

#include "CombatPlayerController.h"
#include "Combat/HUD/CombatHUD.h"
#include "Combat/HUD/CombatWidget.h"


void ACombatPlayerController::BeginPlay()
{
	Super::BeginPlay();
	CombatHUD = Cast<ACombatHUD>(GetHUD());
}

void ACombatPlayerController::CreateCombatWidget()
{
	if (CombatHUD)
	{
		CombatHUD->CreateCombatWidget();
	}

}

void ACombatPlayerController::AddCombatWidgetToViewport()
{
	if (CombatHUD)
	{
		CombatHUD->AddWidgetToViewport(CombatHUD->GetCombatWidget());
	}
}

void ACombatPlayerController::UpdateHealth_HUD(const float& NewHealth, const float& MaxHealth)
{
	if (CombatHUD && CombatHUD->GetCombatWidget())
	{
		CombatHUD->GetCombatWidget()->UpdateHealth_HUD(NewHealth, MaxHealth);
	}
}

void ACombatPlayerController::UpdateEnergy_HUD(const float& NewEnergy, const float& MaxEnergy)
{
	if (CombatHUD && CombatHUD->GetCombatWidget())
	{
		CombatHUD->GetCombatWidget()->UpdateEnergy_HUD(NewEnergy, MaxEnergy);
	}
}

