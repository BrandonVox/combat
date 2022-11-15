// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Combat/Types/AttackType.h"
#include "Combat/Types/SpeedMode.h"
#include "Combat/Interfaces/AttackableInterface.h"
#include "CombatCharacter.generated.h"



class UCombatComponent;
class UStatsComponent;
class UCollisionComponent;

class USoundCue;
class UParticleSystem;
class UAnimMontage;

UCLASS()
class COMBAT_API ACombatCharacter : public ACharacter, public IAttackableInterface
{
	GENERATED_BODY()

public:
	ACombatCharacter();
	// virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

	UCombatComponent* GetCombat_Implementation() const;
	UCollisionComponent* GetCollision_Implementation() const;

	UFUNCTION()
	void OnHitActor(const FHitResult& HitResult);

	UFUNCTION()
		void OnReceivedPointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy,
			FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName,
			FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);





	// HUD
	virtual void UpdateHealth_HUD(const float& NewHealth, const float& MaxHealth);
	virtual void UpdateEnergy_HUD(const float& NewEnergy, const float& MaxEnergy);

	float GetSpeed();

	void DecreaseEnergyByAttackType(EAttackType AttackType);

	bool HasEnoughEnergyForThisAttackType(EAttackType AttackType);
	bool HasEnoughEnergyForDefend();


	const float GetDamageOfLastAttack();

	void HandleDeadTimerFinished();

	void Sprint();
	void Jog();

	bool IsDead();

	const bool IsAttacking();
	const bool IsDefending();
protected:
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)




	virtual void HandleHitted(const FVector& HitLocation, const FVector& ShotFromDirection);
	virtual void HandleDead(const FVector& HitLocation);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStatsComponent* StatsComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCombatComponent* CombatComponent;

	UPROPERTY()
	ESpeedMode SpeedMode;
private:








	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCollisionComponent* CollisionComponent;


	// Defend
	UPROPERTY(EditAnywhere, Category = Defend)
	USoundCue* DefendSound;
	UPROPERTY(EditAnywhere, Category = Defend)
	UParticleSystem* DefendHitImpact;


	// Hitted
	UPROPERTY(EditAnywhere, Category = Hitted)
	USoundCue* HitSound;
	UPROPERTY(EditAnywhere, Category = Hitted)
	UParticleSystem* HitImpact;

	UPROPERTY(EditAnywhere, Category = Hitted)
	UAnimMontage* HitReactMontage_Behind;

	UPROPERTY(EditAnywhere, Category = Hitted)
	UAnimMontage* HitReactMontage_Front;

	// Dead
	UPROPERTY(EditAnywhere, Category = Dead)
	USoundCue* DeadSound;
	UPROPERTY(EditAnywhere, Category = Dead)
	UAnimMontage* DeadMontage;

	UPROPERTY()
	FTimerHandle DeadTimer;

	UPROPERTY(EditAnywhere, Category = Dead)
	float DeadTime = 2.f;



	UPROPERTY()
	FVector Velocity;

	// Sprint, Jog


	UPROPERTY(EditAnywhere, Category = Movement)
	float SprintSpeed = 900.f;
	UPROPERTY(EditAnywhere, Category = Movement)
	float JogSpeed = 500.f;

// Setters and Getters
public:	
	FORCEINLINE UFUNCTION(BlueprintCallable) UCombatComponent* GetCombatComponent() const { return CombatComponent; }
	FORCEINLINE bool IsSprinting() { return SpeedMode == ESpeedMode::ESM_Sprint && GetSpeed() > 0.f; }
	FORCEINLINE void SetSpeedMode(const ESpeedMode& Value) { SpeedMode = Value; }
	FORCEINLINE ESpeedMode GetSpeedMode() const { return SpeedMode; }
};
