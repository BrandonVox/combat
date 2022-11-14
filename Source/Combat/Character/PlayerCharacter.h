
#pragma once

#include "CoreMinimal.h"
#include "CombatCharacter.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UFocusComponent;
class ACombatPlayerController;

UCLASS()
class COMBAT_API APlayerCharacter : public ACombatCharacter
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

	void SetControllerRotation(FRotator NewControllerRotation);
protected:
	virtual void BeginPlay() override;
	// Pressed
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

	// Charge Attack
	void HandleChargeTimerFinish();



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

