
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Combat/Types/StatName.h"
#include "Combat/Types/AttackType.h"
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
	
	void DecreaseEnergyByAttackType(EAttackType AttackType);
	
	void DecreaseHealth(const float& AmmountHealth);
	void DecreaseEnergy(const float& AmmountEnergy);

	bool HasEnoughEnergyForThisAttackType(EAttackType AttackType);
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

	// Sprint
	UPROPERTY(EditAnywhere)
	float EnergyCost_Sprint = 20.f;

	UPROPERTY(EditAnywhere)
	float EnergyRegenPerSecond = 20.f;

	// Defend
	UPROPERTY(EditAnywhere)
	float EnergyCost_Defend = 20.f;

	// So nang luong ton khi dung 1 loai tan cong nao do
	UPROPERTY(EditAnywhere)
	TMap<EAttackType, float> EnergyCostMap_Attack;

public:	
	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE float GetEnergy() const { return Energy; }
		
	FORCEINLINE const float GetMaxHealth() { return StatMap[EStatName::ESN_Health].MaxValue; }
	FORCEINLINE float GetMaxEnergy() const { return StatMap[EStatName::ESN_Energy].MaxValue; }

	FORCEINLINE void SetCombatCharacter(ACombatCharacter* Value) { CombatCharacter = Value; }

	FORCEINLINE float GetEnergyCost_Defend() const { return EnergyCost_Defend; }
};
