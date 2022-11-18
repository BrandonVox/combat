
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
	virtual void UpdateEnergy_HUD(const float& NewEnergy, const float& MaxEnergy) override;
	UFUNCTION(BlueprintCallable)
	void ChangeMaxWalkSpeed(const float& NewValue);

	void OnHitActor(const FHitResult& HitResult) override;
protected:
	virtual void BeginPlay() override;
	virtual void HandleDead(const FVector& HitLocation) override;

private:
	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* WidgetComponent_Health;

	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* WidgetComponent_Energy;

	UPROPERTY()
	UHealthWidget* HealthWidget;

	UPROPERTY()
	UHealthWidget* EnergyWidget;
};
