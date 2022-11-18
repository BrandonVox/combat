

#include "CombatPlayerController.h"
#include "Combat/HUD/CombatHUD.h"
#include "Combat/HUD/CombatWidget.h"


void ACombatPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("OpenMenu", IE_Pressed, this, &ACombatPlayerController::OpenMenuButtonPressed);
}

void ACombatPlayerController::BeginPlay()
{
	Super::BeginPlay();
	CombatHUD = Cast<ACombatHUD>(GetHUD());

	// Menu Widget
	if (CombatHUD)
	{
		CombatHUD->CreateMenuWidget();
	}
}

void ACombatPlayerController::OpenMenuButtonPressed()
{
	if (CombatHUD && CombatHUD->GetMenuWidget())
	{
		CombatHUD->GetMenuWidget()->AddToViewport();
		// sau khi hien menu thi minh muon hien con tro chuot
		// focus mode -> UI
		SetInputMode(FInputModeUIOnly());
		SetShowMouseCursor(true);
	}
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

void ACombatPlayerController::ShowDefeatWidget()
{
	if (CombatHUD)
	{
		CombatHUD->CreateDefeatWidget(this);
		CombatHUD->ShowDefeatWidget(this);
	}
}

