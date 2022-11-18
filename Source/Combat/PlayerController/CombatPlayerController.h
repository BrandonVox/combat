
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CombatPlayerController.generated.h"

class UUserWidget;
class ACombatHUD;
UCLASS()
class COMBAT_API ACombatPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void CreateCombatWidget();
	void AddCombatWidgetToViewport();

	void UpdateHealth_HUD(const float& NewHealth, const float& MaxHealth);
	void UpdateEnergy_HUD(const float& NewEnergy, const float& MaxEnergy);

	void ShowDefeatWidget();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	virtual void OpenMenuButtonPressed();

private:
	// HUD
	UPROPERTY()
	ACombatHUD* CombatHUD;
	
};
