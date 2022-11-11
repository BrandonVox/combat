
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
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* WidgetComponent;

	UPROPERTY()
	UHealthWidget* HealthWidget;
};
