// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatHUD.h"
#include "CombatWidget.h"


void ACombatHUD::CreateCombatWidget()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && CombatWidgetClass)
	{
		CombatWidget = CreateWidget<UCombatWidget>(PlayerController, CombatWidgetClass);
	}
}

void ACombatHUD::AddWidgetToViewport(UUserWidget* WidgetToShow)
{
	if (WidgetToShow)
	{
		WidgetToShow->AddToViewport();
	}
}

void ACombatHUD::CreateMenuWidget()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && MenuWidgetClass)
	{
		MenuWidget = CreateWidget<UUserWidget>(PlayerController, MenuWidgetClass);
	}
}

void ACombatHUD::CreateDefeatWidget(APlayerController* PlayerController)
{
	if (PlayerController && DefeatWidgetClass)
	{
		DefeatWidget = CreateWidget<UUserWidget>(PlayerController, DefeatWidgetClass);
	}
}

void ACombatHUD::ShowDefeatWidget(APlayerController* PlayerController)
{
	if (DefeatWidget && PlayerController)
	{
		DefeatWidget->AddToViewport();
		PlayerController->SetInputMode(FInputModeUIOnly());
		PlayerController->SetShowMouseCursor(true);
	}
}
