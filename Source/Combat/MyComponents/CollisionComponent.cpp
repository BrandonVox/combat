

#include "CollisionComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"
#include "Components/SceneComponent.h"


UCollisionComponent::UCollisionComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

}


void UCollisionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner())
	{
		ActorsToIgnore.Emplace(GetOwner());
	}
}



void UCollisionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsEnablingCollision)
	{
		TraceCollision();
	}

}

void UCollisionComponent::EnableCollision()
{
	bIsEnablingCollision = true;
}

void UCollisionComponent::DisableCollision()
{
	bIsEnablingCollision = false;

}

void UCollisionComponent::TraceCollision()
{
	if (Character == nullptr)
	{
		return;
	}

	TArray<FHitResult> HitResults;

	FVector StartLocation = Character->GetMesh()->GetSocketLocation(StartSocketName);
	FVector EndLocation = Character->GetMesh()->GetSocketLocation(EndSocketName);

	// Start of Sword -> End of Sword
	UKismetSystemLibrary::SphereTraceMultiForObjects
	(
		Character,
		StartLocation,
		EndLocation,
		TraceRadius,
		TraceObjectTypes,
		true,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResults,
		true
	);

}

