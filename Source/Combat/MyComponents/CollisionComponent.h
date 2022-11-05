
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CollisionComponent.generated.h"


class ACharacter;
class AActor;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMBAT_API UCollisionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCollisionComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UFUNCTION(BlueprintCallable)
	void EnableCollision();

	UFUNCTION(BlueprintCallable)
	void DisableCollision();

	UFUNCTION(BlueprintCallable)
	void TraceCollision();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	ACharacter* Character;

	bool bIsEnablingCollision = false;

	UPROPERTY(EditAnywhere)
	FName StartSocketName;
	UPROPERTY(EditAnywhere)
	FName EndSocketName;

	UPROPERTY(EditAnywhere)
	float TraceRadius = 20.f;
	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	UPROPERTY(EditAnywhere)
	TArray<AActor*> ActorsToIgnore;

public:	
	FORCEINLINE void SetCharacter(ACharacter* Value) { Character = Value; }
};
