

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

