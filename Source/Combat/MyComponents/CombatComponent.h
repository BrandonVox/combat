
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Combat/Types/CombatState.h"
#include "CombatComponent.generated.h"

class UAnimMontage;
class ACharacter;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMBAT_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	// virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void RequestAttack();

	void PlayAnimMontage(UAnimMontage* MontageToPlay);

protected:
	virtual void BeginPlay() override;
	virtual void Attack();


private:
	UPROPERTY()
	ECombatState CombatState;

	UPROPERTY(EditAnywhere, Category = Animations)
	TArray<UAnimMontage*> AttackAnimMontages;

	UPROPERTY()
	ACharacter* Character;

// Getters and Setters
public:	
	FORCEINLINE void SetCharacter(ACharacter* Value) { Character = Value; }
		
};
