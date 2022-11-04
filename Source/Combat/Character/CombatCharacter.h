// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Combat/Types/SpeedMode.h"
#include "CombatCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UCombatComponent;
UCLASS()
class COMBAT_API ACombatCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACombatCharacter();
	// virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	virtual void AttackButtonPressed();
	virtual void SprintButtonPressed();

	// Released
	virtual void SprintButtonReleased();
	// Axises
	virtual void MoveForward(float Value);
	virtual void MoveRight(float Value);
	virtual void LookUp(float Value);
	virtual void Turn(float Value);

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

	UPROPERTY(VisibleAnywhere)
	UCombatComponent* CombatComponent;

// Setters and Getters
public:	



};
