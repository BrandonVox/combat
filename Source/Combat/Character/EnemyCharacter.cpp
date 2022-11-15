
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
	WidgetComponent_Health->AddLocalOffset( FVector(0.f, 0.f, 142.f));


	// Energy
	WidgetComponent_Energy =
		CreateDefaultSubobject<UWidgetComponent>(TEXT("EnergyWidgetComponent"));
	WidgetComponent_Energy->SetupAttachment(RootComponent);

	WidgetComponent_Energy->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComponent_Energy->SetDrawAtDesiredSize(true);
	WidgetComponent_Energy->AddLocalOffset(FVector(0.f, 0.f, 130.f));

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


	if (WidgetComponent_Energy)
	{
		EnergyWidget = Cast<UHealthWidget>(WidgetComponent_Energy->GetWidget());
		UpdateEnergy_HUD(StatsComponent->GetEnergy(), StatsComponent->GetMaxEnergy());
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

void AEnemyCharacter::UpdateEnergy_HUD(const float& NewEnergy, const float& MaxEnergy)
{
	if (EnergyWidget)
	{
		EnergyWidget->UpdateHealth_HUD(NewEnergy, MaxEnergy);
	}
}

void AEnemyCharacter::ChangeMaxWalkSpeed(const float& NewValue)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NewValue;
	}
}



void AEnemyCharacter::HandleDead(const FVector& HitLocation)
{
	if (WidgetComponent_Health)
	{
		WidgetComponent_Health->SetHiddenInGame(true);
	}

	Super::HandleDead(HitLocation);
}
