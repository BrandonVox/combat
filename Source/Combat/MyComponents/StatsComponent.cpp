

#include "StatsComponent.h"

UStatsComponent::UStatsComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

	// Init Stat map
	StatMap.Emplace(EStatName::ESN_Health, FStatValue());
	StatMap.Emplace(EStatName::ESN_Energy, FStatValue());
}

void UStatsComponent::InitStatValues()
{
	Health = StatMap[EStatName::ESN_Health].BaseValue;
	Energy = StatMap[EStatName::ESN_Energy].BaseValue;
}

float UStatsComponent::GetStatValue(EStatName StatNameToGet)
{
	switch (StatNameToGet)
	{
	case EStatName::ESN_Health:
		return Health;
	case EStatName::ESN_Energy:
		return Energy;
	}

	return 0.0f;
}

float UStatsComponent::GetMaxStatValue(EStatName StatNameToGet)
{
	switch (StatNameToGet)
	{
	case EStatName::ESN_Health:
		return StatMap[EStatName::ESN_Health].MaxValue;
	case EStatName::ESN_Energy:
		return StatMap[EStatName::ESN_Energy].MaxValue;
	}

	return 0.0f;
}


void UStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

