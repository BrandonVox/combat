
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Combat/Types/StatName.h"
#include "StatsComponent.generated.h"

USTRUCT(BlueprintType)
struct FStatValue
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float BaseValue;
	UPROPERTY(EditAnywhere)
	float MaxValue;
};

class ACombatCharacter;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMBAT_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStatsComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	bool CanRegenEnergy();
	void InitStatValues();

	float GetStatValue(EStatName StatNameToGet);
	float GetMaxStatValue(EStatName StatNameToGet);
	

protected:
	virtual void BeginPlay() override;

	void HandleSprinting(const float& DeltaTime);
	void RegenEnergy(const float& DeltaTime);

private:
	float Health = 0.f;
	float Energy = 0.f;

	UPROPERTY(EditAnywhere)
	TMap<EStatName, FStatValue> StatMap;

	UPROPERTY()
	ACombatCharacter* CombatCharacter;

	UPROPERTY(EditAnywhere)
	float EnergyCost_Sprint = 20.f;

	UPROPERTY(EditAnywhere)
	float EnergyRegenPerSecond = 20.f;

public:	
	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE float GetEnergy() const { return Energy; }
		
	FORCEINLINE float GetMaxHealth() const { return StatMap[EStatName::ESN_Health].MaxValue; }
	FORCEINLINE float GetMaxEnergy() const { return StatMap[EStatName::ESN_Energy].MaxValue; }

	FORCEINLINE void SetCombatCharacter(ACombatCharacter* Value) { CombatCharacter = Value; }
};
