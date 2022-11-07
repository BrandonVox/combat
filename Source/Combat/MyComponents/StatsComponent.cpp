

#include "StatsComponent.h"
#include "Combat/Character/CombatCharacter.h"

UStatsComponent::UStatsComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

	// Init Stat map
	StatMap.Emplace(EStatName::ESN_Health, FStatValue());
	StatMap.Emplace(EStatName::ESN_Energy, FStatValue());
}

void UStatsComponent::InitStatValues()
{
	Health = StatMap[EStatName::ESN_Health].BaseValue;
	Energy = StatMap[EStatName::ESN_Energy].BaseValue;
}

void UStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	if (CombatCharacter == nullptr)
	{
		CombatCharacter = Cast<ACombatCharacter>(GetOwner());
	}
}


void UStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CombatCharacter && CombatCharacter->IsSprinting())
	{
		HandleSprinting(DeltaTime);
	}

	if (CombatCharacter && CombatCharacter->IsSprinting() == false)
	{
		RegenEnergy(DeltaTime);
	}

}

void UStatsComponent::HandleSprinting(const float& DeltaTime)
{
	// Update Value
	float NewEnergy = Energy - EnergyCost_Sprint * DeltaTime;
	Energy = FMath::Clamp(NewEnergy, 0.f, GetMaxEnergy());

	// Update HUD
	CombatCharacter->UpdateEnergy_HUD(Energy, GetMaxEnergy());

	// If Energy = 0, -> Jog
	if (Energy <= 0.f)
	{
		CombatCharacter->Jog();
	}
}

void UStatsComponent::RegenEnergy(const float& DeltaTime)
{
	// Update Value
	float NewEnergy = Energy + EnergyRegenPerSecond * DeltaTime;
	Energy = FMath::Clamp(NewEnergy, 0.f, GetMaxEnergy());

	// Update HUD
	CombatCharacter->UpdateEnergy_HUD(Energy, GetMaxEnergy());
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




