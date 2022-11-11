// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Combat/Types/SpeedMode.h"
#include "Combat/Types/AttackType.h"
#include "Combat/Interfaces/AttackableInterface.h"
#include "CombatCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UCombatComponent;
class UStatsComponent;
class UFocusComponent;
class UCollisionComponent;

class USoundCue;
class UParticleSystem;
class UAnimMontage;
class ACombatPlayerController;

UCLASS()
class COMBAT_API ACombatCharacter : public ACharacter, public IAttackableInterface
{
	GENERATED_BODY()

public:
	ACombatCharacter();
	// virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;

	UCombatComponent* GetCombat_Implementation() const;
	UCollisionComponent* GetCollision_Implementation() const;

	UFUNCTION()
	void OnHitActor(const FHitResult& HitResult);

	UFUNCTION()
		void OnReceivedPointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy,
			FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName,
			FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);

	void Sprint();
	void Jog();

	// HUD
	virtual void UpdateHealth_HUD(const float& NewHealth, const float& MaxHealth);
	virtual void UpdateEnergy_HUD(const float& NewEnergy, const float& MaxEnergy);

	float GetSpeed();

	void DecreaseEnergyByAttackType(EAttackType AttackType);

	bool HasEnoughEnergyForThisAttackType(EAttackType AttackType);

	const FVector GetCameraDirection();

	void SetControllerRotation(FRotator NewControllerRotation);

	const float GetDamageOfLastAttack();
protected:
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	virtual void AttackButtonPressed();
	virtual void StrongAttackButtonPressed();
	virtual void ChargeAttackButtonPressed();
	virtual void SprintButtonPressed();
	virtual void FocusButtonPressed();

	// Released
	virtual void SprintButtonReleased();
	virtual void ChargeAttackButtonReleased();
	// Axises
	virtual void MoveForward(float Value);
	virtual void MoveRight(float Value);
	virtual void LookUp(float Value);
	virtual void Turn(float Value);

	void HandleChargeTimerFinish();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStatsComponent* StatsComponent;

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* FollowCamera;

	UPROPERTY()
	ESpeedMode SpeedMode;

	UPROPERTY(EditAnywhere, Category = Movement)
	float SprintSpeed = 900.f;
	UPROPERTY(EditAnywhere, Category = Movement)
	float JogSpeed = 500.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCombatComponent* CombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCollisionComponent* CollisionComponent;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UFocusComponent* FocusComponent;

	// Hitted
	UPROPERTY(EditAnywhere, Category = Hitted)
	USoundCue* HitSound;
	UPROPERTY(EditAnywhere, Category = Hitted)
	UParticleSystem* HitImpact;
	UPROPERTY(EditAnywhere, Category = Hitted)
	UAnimMontage* HitReactMontage;

	UPROPERTY()
	ACombatPlayerController* CombatPlayerController;

	UPROPERTY()
	FVector Velocity;

	UPROPERTY()
	FTimerHandle ChargeAttackTimer;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ChargeTime = 1.f;
// Setters and Getters
public:	
	FORCEINLINE UFUNCTION(BlueprintCallable) UCombatComponent* GetCombatComponent() const { return CombatComponent; }
	FORCEINLINE bool IsSprinting() { return SpeedMode == ESpeedMode::ESM_Sprint && GetSpeed() > 0.f; }

	FORCEINLINE void SetSpeedMode(const ESpeedMode& Value) { SpeedMode = Value; }
};
