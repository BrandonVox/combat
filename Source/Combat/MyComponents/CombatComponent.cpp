

#include "CombatComponent.h"
#include "GameFramework/Character.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	ResetCombat();
}

void UCombatComponent::RequestAttack(const EAttackType& AttackType)
{
	if (CanAttack())
	{
		bIsReachedContinueAttackPoint = false;
		Attack(AttackType);
	}
}

bool UCombatComponent::CanAttack()
{
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

void UCombatComponent::ContinueCombo()
{
	bIsReachedContinueAttackPoint = true;
}
