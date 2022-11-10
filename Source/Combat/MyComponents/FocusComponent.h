
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FocusComponent.generated.h"

class AActor;
class ACombatCharacter;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMBAT_API UFocusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFocusComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Focus();
	void UnFocus();

	void FaceCameraAtTarget();

	void FindTarget();

protected:
	virtual void BeginPlay() override;

private:
	bool bIsFocusing = false;

	UPROPERTY()
	AActor* TargetActor;

	UPROPERTY()
	ACombatCharacter* CombatCharacter;

	// Trace
	UPROPERTY(EditAnywhere)
	float TraceRadius = 20.f;

	UPROPERTY(EditAnywhere)
	float TraceLength = 500.f;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> ActorsToIgnore;

	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	UPROPERTY(EditAnywhere)
	bool bDebug;

	UPROPERTY(EditAnywhere)
	float Offset_Z = 50.f;

	UPROPERTY(EditAnywhere)
	float MaxFocusLength = 500.f;

public:	
	FORCEINLINE const bool IsFocusing() { return bIsFocusing; }
	FORCEINLINE void SetCombatCharacter(ACombatCharacter* Value) { CombatCharacter = Value; }
		
};
