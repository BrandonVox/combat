
#include "EnemyCharacter.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Combat/HUD/HealthWidget.h"
#include "Combat/MyComponents/StatsComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	WidgetComponent = 
		CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(RootComponent);

	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComponent->SetDrawAtDesiredSize(true);
	WidgetComponent->AddLocalOffset( FVector(0.f, 0.f, 130.f));


	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()
			->SetCollisionResponseToChannel
			(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
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

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// minh da co widget class roi
	// minh khong can phai tao widget
	if (WidgetComponent)
	{
		HealthWidget = Cast<UHealthWidget>(WidgetComponent->GetWidget());
		if (HealthWidget)
		{
			HealthWidget->UpdateHealth_HUD(StatsComponent->GetHealth(), StatsComponent->GetMaxHealth());
		}
	}

}