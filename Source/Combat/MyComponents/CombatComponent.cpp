

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
	CombatState = ECombatState::ECS_Free;
}

void UCombatComponent::RequestAttack()
{
	if (CombatState == ECombatState::ECS_Free && AttackAnimMontages.IsEmpty() == false)
	{
		Attack();
	}
}

void UCombatComponent::Attack()
{
	UAnimMontage* MontageToPlay = AttackAnimMontages[0];
	if (MontageToPlay)
	{
		PlayAnimMontage(MontageToPlay);
		CombatState = ECombatState::ECS_Attack;
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