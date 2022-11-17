// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CombatHUD.generated.h"

/**
 * 
 */
class UCombatWidget;
class UUserWidget;
UCLASS()
class COMBAT_API ACombatHUD : public AHUD
{
	GENERATED_BODY()

public:
	void CreateCombatWidget();
	void AddWidgetToViewport(UUserWidget* WidgetToShow);

	void CreateMenuWidget();
private:
	// Combat
	UPROPERTY(EditAnywhere, Category = Combat)
	TSubclassOf<UUserWidget> CombatWidgetClass;

	UPROPERTY()
	UCombatWidget* CombatWidget;

	// Menu
	UPROPERTY(EditAnywhere, Category = Menu)
	TSubclassOf<UUserWidget> MenuWidgetClass;

	UPROPERTY()
	UUserWidget* MenuWidget;
public:
	FORCEINLINE UCombatWidget* GetCombatWidget() const { return CombatWidget; }
	FORCEINLINE UUserWidget* GetMenuWidget() const { return MenuWidget; }

};
