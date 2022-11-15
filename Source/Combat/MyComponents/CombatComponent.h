
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Combat/Types/CombatState.h"
#include "Combat/Types/AttackType.h"
#include "CombatComponent.generated.h"

class UAnimMontage;
class ACombatCharacter;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMBAT_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	// virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void RequestAttack(const EAttackType& AttackType);

	UFUNCTION(BlueprintCallable)
	void RequestDefend();



	void PlayAnimMontage(UAnimMontage* MontageToPlay);

	UFUNCTION(BlueprintCallable)
	void ResetCombat();

	UFUNCTION(BlueprintCallable)
	void ContinueCombo();

	const float GetDamageByAttackType(const EAttackType& AttackType);
	const float GetDamageOfLastAttack();

protected:
	virtual void BeginPlay() override;

	virtual bool CanAttack(EAttackType AttackType);
	virtual void Attack(const EAttackType& AttackType);

	// Defend
	virtual bool CanDefend();
	virtual void Defend();

	UAnimMontage* GetAttackMontage(const EAttackType& AttackType);

private:
	UPROPERTY()
	ECombatState CombatState;

	// Attack Montages
	UPROPERTY(EditAnywhere, Category = AttackAnimations)
	TArray<UAnimMontage*> LightAttackMontages;

	UPROPERTY(EditAnywhere, Category = AttackAnimations)
	TArray<UAnimMontage*> StrongAttackMontages;

	UPROPERTY(EditAnywhere, Category = AttackAnimations)
	TArray<UAnimMontage*> SprintAttackMontages;

	UPROPERTY(EditAnywhere, Category = AttackAnimations)
	UAnimMontage* ChargeAttackMontage;

	// Defend Montage
	UPROPERTY(EditAnywhere, Category = DefendAnimation)
	UAnimMontage* DefendMontage;

	UPROPERTY()
	ACombatCharacter* CombatCharacter;

	bool bIsReachedContinueAttackPoint = false;
	int32 AttackIndex = 0;

	UPROPERTY()
	EAttackType LastAttackType;

	UPROPERTY(EditAnywhere, Category = Damage)
	TMap<EAttackType, float> DamageMap;
// Getters and Setters
public:	
	FORCEINLINE void SetCombatCharacter(ACombatCharacter* Value) { CombatCharacter = Value; }
		
	FORCEINLINE void SetCombatState(ECombatState Value) { CombatState = Value; }
	FORCEINLINE ECombatState GetCombatState() const { return CombatState; }
	FORCEINLINE EAttackType GetLastAttackType() const { return LastAttackType;}
};
