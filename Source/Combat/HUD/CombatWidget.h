

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombatWidget.generated.h"


class UProgressBar;
UCLASS()
class COMBAT_API UCombatWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdateHealth_HUD(float NewHealth, float MaxHealth);
	void UpdateEnergy_HUD(float NewEnergy, float MaxEnergy);
private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* EnergyBar;
};
