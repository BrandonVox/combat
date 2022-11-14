

#include "FocusComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Combat/Character/PlayerCharacter.h"

UFocusComponent::UFocusComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFocusComponent::BeginPlay()
{
	Super::BeginPlay();

	// rat kho xay ra
	if (PlayerCharacter == nullptr)
	{
		PlayerCharacter = Cast<APlayerCharacter>( GetOwner());
	}

	// thuong xay ra
	if (PlayerCharacter)
	{
		ActorsToIgnore.Emplace(PlayerCharacter);
	}
}

void UFocusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// o moi frame, neu nguoi choi focus thi minh se huong camera toi
	// muc tieu
	if (bIsFocusing)
	{
		FaceCameraAtTarget();
	}
}

void UFocusComponent::Focus()
{
	// phai tim target da
	FindTarget();
}

void UFocusComponent::UnFocus()
{
	bIsFocusing = false;
	TargetActor = nullptr;
	// khong cho camera huong toi muc tieu nua
}

void UFocusComponent::FaceCameraAtTarget()
{
	// huong camera den muc tieu
	if (TargetActor == nullptr)
	{
		UnFocus();
		return;
	}
	
	if (PlayerCharacter == nullptr)
	{
		UnFocus();
		return;
	}

	FVector StartLocation = PlayerCharacter->GetActorLocation();
	StartLocation.Z += Offset_Z;

	FRotator NewControllerRotation =
		(TargetActor->GetActorLocation() - StartLocation).Rotation();

	PlayerCharacter->SetControllerRotation(NewControllerRotation);

	// neu khoang cach tu nguoi choi den 
	// muc tieu qua xa thi khong cho target nua
	const float DistanceToTarget = PlayerCharacter->GetDistanceTo(TargetActor);
	if (DistanceToTarget > MaxFocusLength)
	{
		UnFocus();
	}
}

void UFocusComponent::FindTarget()
{
	// sphere trace de tim muc tieu
	if (PlayerCharacter == nullptr)
	{
		return;
	}

	FHitResult HitResult;

	// character->getcameradirection
	const FVector StartLocation = PlayerCharacter->GetActorLocation();
	FVector EndLocation = 
		StartLocation + PlayerCharacter->GetCameraDirection() * TraceLength;

	// Start of Sword -> End of Sword
	UKismetSystemLibrary::SphereTraceSingleForObjects
	(
		PlayerCharacter,
		StartLocation,
		EndLocation,
		TraceRadius,
		TraceObjectTypes,
		true,
		ActorsToIgnore,
		bDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		HitResult,
		true
	);


	if (HitResult.bBlockingHit && HitResult.GetActor())
	{
		TargetActor = HitResult.GetActor();
		bIsFocusing = true;
	}


}

