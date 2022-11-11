
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthWidget.generated.h"

class UProgressBar;
UCLASS()
class COMBAT_API UHealthWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateHealth_HUD(const float& NewHealth, const float& MaxHealth);

private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

};
