
#pragma once

#include "CoreMinimal.h"
#include "CombatCharacter.h"
#include "EnemyCharacter.generated.h"

class UWidgetComponent;
class UHealthWidget;
UCLASS()
class COMBAT_API AEnemyCharacter : public ACombatCharacter
{
	GENERATED_BODY()
public:
	AEnemyCharacter();
	virtual void UpdateHealth_HUD(const float& NewHealth, const float& MaxHealth) override;
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* WidgetComponent;

	UPROPERTY()
	UHealthWidget* HealthWidget;
};
