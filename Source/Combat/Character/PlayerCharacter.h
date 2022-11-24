
#pragma once

#include "CoreMinimal.h"
#include "CombatCharacter.h"
#include "Combat/Interfaces/UseEnergyInterface.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UFocusComponent;
class ACombatPlayerController;

UCLASS()
class COMBAT_API APlayerCharacter : public ACombatCharacter, public IUseEnergyInterface
{
	GENERATED_BODY()
public:
	APlayerCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;

	const FVector GetCameraDirection();

	// HUD
	virtual void UpdateHealth_HUD(const float& NewHealth, const float& MaxHealth) override;
	virtual void UpdateEnergy_HUD(const float& NewEnergy, const float& MaxEnergy) override;

	void SetControllerRotation(const FRotator& NewControllerRotation);

	void SetupFocus(const bool& bDoFocus);

	virtual void OnReceivedPointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy,
		FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName,
		FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser) override;


	// Interfaces
	// Use Energy Interface
	virtual bool HasEnoughEnergyForThisAttackType(const EAttackType& AttackType) override;
	virtual void DecreaseEnergyByAttackType(const EAttackType& AttackType) override;
	virtual bool HasEnoughEnergyForDefend() override;

protected:
	virtual void BeginPlay() override;
	// Pressed
	virtual void AttackButtonPressed();
	virtual void StrongAttackButtonPressed();
	virtual void ChargeAttackButtonPressed();
	virtual void SprintButtonPressed();
	virtual void FocusButtonPressed();
	virtual void DefendButtonPressed();

	// Released
	virtual void SprintButtonReleased();
	virtual void ChargeAttackButtonReleased();
	// Axises
	virtual void MoveForward(float Value);
	virtual void MoveRight(float Value);
	virtual void LookUp(float Value);
	virtual void Turn(float Value);

	// Charge Attack
	void HandleChargeTimerFinish();

	virtual void HandleDead(const FVector& HitLocation) override;

private:
	UPROPERTY()
	ACombatPlayerController* CombatPlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UFocusComponent* FocusComponent;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* FollowCamera;



	// Charge Attack
	UPROPERTY()
	FTimerHandle ChargeAttackTimer;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ChargeTime = 1.f;



public:

};

