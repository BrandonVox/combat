
#include "EnemyCharacter.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Combat/HUD/HealthWidget.h"
#include "Combat/MyComponents/StatsComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	// Health
	WidgetComponent_Health = 
		CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidgetComponent"));
	WidgetComponent_Health->SetupAttachment(RootComponent);

	WidgetComponent_Health->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComponent_Health->SetDrawAtDesiredSize(true);
	WidgetComponent_Health->AddLocalOffset( FVector(0.f, 0.f, 120.f));

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = 200.f;
	}

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()
			->SetCollisionResponseToChannel
			(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	}
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// minh da co widget class roi
	// minh khong can phai tao widget
	if (WidgetComponent_Health)
	{
		HealthWidget = Cast<UHealthWidget>(WidgetComponent_Health->GetWidget());
		UpdateHealth_HUD(StatsComponent->GetHealth(), StatsComponent->GetMaxHealth());
	}

}

void AEnemyCharacter::UpdateHealth_HUD(const float& NewHealth, const float& MaxHealth)
{
	// enemy cho nen khong co hud
	if (HealthWidget)
	{
		HealthWidget->UpdateHealth_HUD(NewHealth, MaxHealth);
	}
}



void AEnemyCharacter::ChangeMaxWalkSpeed(const float& NewValue)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NewValue;
	}
}

void AEnemyCharacter::OnHitActor(const FHitResult& HitResult)
{
	AActor* HittedActor = HitResult.GetActor();
	if (HittedActor && HittedActor->GetClass())
	{
		bool bIsAlly = HittedActor->GetClass()->IsChildOf(AEnemyCharacter::StaticClass());
		if (bIsAlly)
		{
			return;
		}
	}

	Super::OnHitActor(HitResult);
}



void AEnemyCharacter::HandleDead(const FVector& HitLocation)
{
	if (WidgetComponent_Health)
	{
		WidgetComponent_Health->SetHiddenInGame(true);
	}

	Super::HandleDead(HitLocation);
}
