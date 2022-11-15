// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UCombatAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Character = Cast<ACharacter>(TryGetPawnOwner());
}

void UCombatAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Character == nullptr)
	{
		Character = Cast<ACharacter>(TryGetPawnOwner());
	}
	if (Character == nullptr)
	{
		return;
	}

	Velocity = Character->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	bIsAccelerating = Character->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f;

	bIsInAir = Character->GetCharacterMovement()->IsFalling();

	// Strafing
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(Velocity);
	FRotator AimRotation = Character->GetBaseAimRotation();
	Strafing_Yaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;
}
