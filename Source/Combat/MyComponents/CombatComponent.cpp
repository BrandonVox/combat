

#include "CombatComponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	ResetCombat();
}

void UCombatComponent::RequestAttack()
{
	if (CanAttack())
	{
		bIsReachedContinueAttackPoint = false;
		Attack();
	}
}

bool UCombatComponent::CanAttack()
{
	bool bDesiredCombatState =
		CombatState == ECombatState::ECS_Free
		|| (bIsReachedContinueAttackPoint && CombatState == ECombatState::ECS_Attack);
	return 
		bDesiredCombatState
		&& AttackAnimMontages.IsEmpty() == false
		;
}

void UCombatComponent::Attack()
{
	UAnimMontage* MontageToPlay = AttackAnimMontages[AttackIndex];
	if (MontageToPlay)
	{
		PlayAnimMontage(MontageToPlay);
		CombatState = ECombatState::ECS_Attack;

		AttackIndex++;
		// 0 1 2 
		if (AttackIndex > AttackAnimMontages.Num() - 1)
		{
			AttackIndex = 0;
		}
	}
}

void UCombatComponent::PlayAnimMontage(UAnimMontage* MontageToPlay)
{
	if (Character == nullptr)
	{
		return;
	}
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	if (AnimInstance && MontageToPlay)
	{
		AnimInstance->Montage_Play(MontageToPlay);
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
