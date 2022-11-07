
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


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMBAT_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStatsComponent();
	// virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void InitStatValues();

	float GetStatValue(EStatName StatNameToGet);
	float GetMaxStatValue(EStatName StatNameToGet);

protected:
	virtual void BeginPlay() override;

private:
	float Health = 0.f;
	float Energy = 0.f;

	UPROPERTY(EditAnywhere)
	TMap<EStatName, FStatValue> StatMap;

public:	
	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE float GetEnergy() const { return Energy; }
		
	FORCEINLINE float GetMaxHealth() const { return StatMap[EStatName::ESN_Health].MaxValue; }
	FORCEINLINE float GetMaxEnergy() const { return StatMap[EStatName::ESN_Energy].MaxValue; }
};
