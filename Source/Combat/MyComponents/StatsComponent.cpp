

#include "StatsComponent.h"
#include "Combat/Character/CombatCharacter.h"

UStatsComponent::UStatsComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

	// Init Stat map
	StatMap.Emplace(EStatName::ESN_Health, FStatValue());
	StatMap.Emplace(EStatName::ESN_Energy, FStatValue());

	// Init Energy Map for Attacks
	EnergyCostMap_Attack.Emplace(EAttackType::EAT_LightAttack, 0.f);

	EnergyCostMap_Attack.Emplace(EAttackType::EAT_StrongAttack, 0.f);
	EnergyCostMap_Attack.Emplace(EAttackType::EAT_SprintAttack, 0.f);
	EnergyCostMap_Attack.Emplace(EAttackType::EAT_ChargeAttack, 0.f);

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

	if (CanRegenEnergy())
	{
		RegenEnergy(DeltaTime);
	}

}

bool UStatsComponent::CanRegenEnergy()
{
	return CombatCharacter 
		&& CombatCharacter->IsSprinting() == false
		&& Energy < GetMaxEnergy();
}

void UStatsComponent::HandleSprinting(const float& DeltaTime)
{
	Energy -= EnergyCost_Sprint * DeltaTime;
	Energy = FMath::Clamp(Energy, 0.f, GetMaxEnergy());

	if (CombatCharacter == nullptr)
	{
		return;
	}

	CombatCharacter->UpdateEnergy_HUD(Energy, GetMaxEnergy());
	if (Energy <= 0.f)
	{
		CombatCharacter->Jog();
	}
}

void UStatsComponent::RegenEnergy(const float& DeltaTime)
{
	Energy += EnergyRegenPerSecond * DeltaTime;
	Energy = FMath::Clamp(Energy, 0.f, GetMaxEnergy());

	if (CombatCharacter)
	{
		CombatCharacter->UpdateEnergy_HUD(Energy, GetMaxEnergy());
	}
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


void UStatsComponent::DecreaseEnergyByAttackType(EAttackType AttackType)
{
	Energy -= EnergyCostMap_Attack[AttackType];
	Energy = FMath::Clamp(Energy, 0.f, GetMaxEnergy());

	if (CombatCharacter)
	{
		CombatCharacter->UpdateEnergy_HUD(Energy, GetMaxEnergy());
	}
}

void UStatsComponent::DecreaseHealth(const float& AmmountHealth)
{
	Health -= AmmountHealth;
	Health = FMath::Clamp(Health, 0.f, GetMaxHealth());

	if (CombatCharacter)
	{
		CombatCharacter->UpdateHealth_HUD(Health, GetMaxHealth());
	}
}

bool UStatsComponent::HasEnoughEnergyForThisAttackType(EAttackType AttackType)
{
	return Energy >= EnergyCostMap_Attack[AttackType];
}


