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
private:
	UPROPERTY(EditAnywhere, Category = Combat)
	TSubclassOf<UUserWidget> CombatWidgetClass;

	UPROPERTY()
	UCombatWidget* CombatWidget;
public:
	FORCEINLINE UCombatWidget* GetCombatWidget() const { return CombatWidget; }
};
