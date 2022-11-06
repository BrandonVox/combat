

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
	HittedActors.Empty();
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
	bool bHitSomething = UKismetSystemLibrary::SphereTraceMultiForObjects
	(
		Character,
		StartLocation,
		EndLocation,
		TraceRadius,
		TraceObjectTypes,
		true,
		ActorsToIgnore,
		bDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		HitResults,
		true
	);

	if (bHitSomething)
	{
		for (FHitResult HR : HitResults)
		{
			if (HR.bBlockingHit)
			{
				if (HR.GetActor())
				{
					// Luu vao mang Hitted Actors
					if (HittedActors.Contains(HR.GetActor()) == false)
					{
						HittedActors.Emplace(HR.GetActor());
						HitActorDelegate.Broadcast(HR);
					}
				}
			}
		}
	}

}

