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
