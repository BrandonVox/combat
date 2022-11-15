// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatAnimInstance.h"
#include "CombatCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UCombatAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	CombatCharacter = Cast<ACombatCharacter>(TryGetPawnOwner());
}

void UCombatAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (CombatCharacter == nullptr)
	{
		CombatCharacter = Cast<ACombatCharacter>(TryGetPawnOwner());
	}
	if (CombatCharacter == nullptr)
	{
		return;
	}

	Velocity = CombatCharacter->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	bIsAccelerating = CombatCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f;

	bIsInAir = CombatCharacter->GetCharacterMovement()->IsFalling();

	// Strafing
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(Velocity);
	FRotator AimRotation = CombatCharacter->GetBaseAimRotation();
	Strafing_Yaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;
	
	// 
	bIsSprinting = CombatCharacter->GetSpeedMode() == ESpeedMode::ESM_Sprint;
}
