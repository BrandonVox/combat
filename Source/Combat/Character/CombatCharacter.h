// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CombatCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
UCLASS()
class COMBAT_API ACombatCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACombatCharacter();
	// virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:
	virtual void BeginPlay() override;
	virtual void AttackButtonPressed();

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
// Setters and Getters
public:	



};
