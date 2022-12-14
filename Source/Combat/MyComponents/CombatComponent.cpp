

#include "CombatComponent.h"
#include "GameFramework/Character.h"
#include "Combat/Interfaces/UseEnergyInterface.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	DamageMap.Emplace(EAttackType::EAT_LightAttack, 0.f);
	DamageMap.Emplace(EAttackType::EAT_StrongAttack, 0.f);
	DamageMap.Emplace(EAttackType::EAT_SprintAttack, 0.f);
	DamageMap.Emplace(EAttackType::EAT_ChargeAttack, 0.f);
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	ResetCombat();
}

void UCombatComponent::RequestAttack(const EAttackType& AttackType)
{
	if (CanAttack(AttackType))
	{
		bIsReachedContinueAttackPoint = false;
		Attack(AttackType);
	}
}

void UCombatComponent::RequestDefend()
{
	if (CanDefend())
	{
		Defend();
	}
}

bool UCombatComponent::CanAttack(EAttackType AttackType)
{
	IUseEnergyInterface* UseEnergyCharacter = Cast<IUseEnergyInterface>(Character);
	if (UseEnergyCharacter)
	{
		if (UseEnergyCharacter->HasEnoughEnergyForThisAttackType(AttackType) == false)
		{
			return false;
		}
	}

	bool bDesiredCombatState =
		CombatState == ECombatState::ECS_Free
		|| (bIsReachedContinueAttackPoint && CombatState == ECombatState::ECS_Attack);
	return
		bDesiredCombatState;

}

void UCombatComponent::Attack(const EAttackType& AttackType)
{
	UAnimMontage* MontageToPlay = GetAttackMontage(AttackType);
	if (MontageToPlay)
	{
		PlayAnimMontage(MontageToPlay);
		CombatState = ECombatState::ECS_Attack;
		LastAttackType = AttackType;

		AttackIndex++;
		// 0 1 2 
	}
}

bool UCombatComponent::CanDefend()
{
	IUseEnergyInterface* UseEnergyCharacter = Cast<IUseEnergyInterface>(Character);
	if (UseEnergyCharacter)
	{
		if (UseEnergyCharacter->HasEnoughEnergyForDefend() == false)
		{
			return false;
		}
	}

	return CombatState == ECombatState::ECS_Free;

}

void UCombatComponent::Defend()
{
	if (DefendMontage)
	{
		PlayAnimMontage(DefendMontage);
		CombatState = ECombatState::ECS_Defend;
	}
}

UAnimMontage* UCombatComponent::GetAttackMontage(const EAttackType& AttackType)
{
	if (LastAttackType != AttackType)
	{
		AttackIndex = 0;
	}

	switch (AttackType)
	{
	case EAttackType::EAT_LightAttack:
		if (LightAttackMontages.IsEmpty())
		{
			return nullptr;
		}
		if (AttackIndex > LightAttackMontages.Num() - 1)
		{
			AttackIndex = 0;
		}
		return LightAttackMontages[AttackIndex];
	case EAttackType::EAT_StrongAttack:
		if (StrongAttackMontages.IsEmpty())
		{
			return nullptr;
		}
		if (AttackIndex > StrongAttackMontages.Num() - 1)
		{
			AttackIndex = 0;
		}
		return StrongAttackMontages[AttackIndex];
	case EAttackType::EAT_SprintAttack:
		if (SprintAttackMontages.IsEmpty())
		{
			return nullptr;
		}
		if (AttackIndex > SprintAttackMontages.Num() - 1)
		{
			AttackIndex = 0;
		}
		return SprintAttackMontages[AttackIndex];

	case EAttackType::EAT_ChargeAttack:
		return ChargeAttackMontage;
	}
	return nullptr;
}

void UCombatComponent::PlayAnimMontage(UAnimMontage* MontageToPlay)
{
	if (Character)
	{
		Character->PlayAnimMontage(MontageToPlay);
	}
}

void UCombatComponent::ResetCombat()
{
	CombatState = ECombatState::ECS_Free;
	AttackIndex = 0;
	bIsReachedContinueAttackPoint = false;
}

// goi khi minh cham toi diem continue combo trong animtion
void UCombatComponent::ContinueCombo()
{
	bIsReachedContinueAttackPoint = true;


	IUseEnergyInterface* UseEnergyCharacter = Cast<IUseEnergyInterface>(Character);
	if (UseEnergyCharacter)
	{
		UseEnergyCharacter->DecreaseEnergyByAttackType(LastAttackType);
	}
}

const float UCombatComponent::GetDamageByAttackType(const EAttackType& AttackType)
{
	return DamageMap[AttackType];
}

const float UCombatComponent::GetDamageOfLastAttack()
{
	return DamageMap[LastAttackType];
}
