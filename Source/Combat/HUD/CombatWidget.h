

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombatWidget.generated.h"


class UProgressBar;
class UTextBlock;
UCLASS()
class COMBAT_API UCombatWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdateHealth_HUD(const float& NewHealth, const float& MaxHealth);
	void UpdateEnergy_HUD(const float& NewEnergy, const float& MaxEnergy);
private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* EnergyBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealthText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaxHealthText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* EnergyText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaxEnergyText;


	UPROPERTY()
	FString NewText;
};
