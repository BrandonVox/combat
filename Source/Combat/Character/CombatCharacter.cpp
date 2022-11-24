

#include "CombatCharacter.h"

#include "Combat/MyComponents/CombatComponent.h"
#include "Combat/MyComponents/CollisionComponent.h"
#include "Combat/MyComponents/StatsComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
//

#include "Components/CapsuleComponent.h"

#include "GameFramework/CharacterMovementComponent.h"


ACombatCharacter::ACombatCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	// Character Configs
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->GravityScale = 1.75f;
	GetCharacterMovement()->AirControl = 0.35f;

	// Components
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	CollisionComponent = CreateDefaultSubobject<UCollisionComponent>(TEXT("CollisionComponent"));
	StatsComponent = CreateDefaultSubobject<UStatsComponent>(TEXT("StatsComponent"));
}



void ACombatCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (CombatComponent)
	{
		CombatComponent->SetCharacter(this);
	}
	if (CollisionComponent)
	{
		CollisionComponent->SetCharacter(this);
		CollisionComponent->HitActorDelegate.AddDynamic(this, &ACombatCharacter::OnHitActor);
	}
	if (StatsComponent)
	{
		StatsComponent->SetCombatCharacter(this);
		StatsComponent->InitStatValues();
	}

}

void ACombatCharacter::BeginPlay()
{
	Super::BeginPlay();

	Jog();

	OnTakePointDamage.AddDynamic(this, &ACombatCharacter::OnReceivedPointDamage);
}

UCombatComponent* ACombatCharacter::GetCombat_Implementation() const
{
	return CombatComponent;
}

UCollisionComponent* ACombatCharacter::GetCollision_Implementation() const
{
	return CollisionComponent;
}


// ham nay chay khi ma minh danh trung ai do
void ACombatCharacter::OnHitActor(const FHitResult& HitResult)
{
	// GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Hit Actor");

	AActor* HittedActor = HitResult.GetActor();

	if (HittedActor)
	{
		UGameplayStatics::ApplyPointDamage(
			HittedActor,
			GetDamageOfLastAttack(),
			GetActorForwardVector(),
			HitResult,
			GetController(),
			this,
			UDamageType::StaticClass()
		);
	}
}

// ham nay chay khi ma minh bi danh
void ACombatCharacter::OnReceivedPointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy,
	FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
	const UDamageType* DamageType, AActor* DamageCauser)
{



	// tru mau
	if (StatsComponent)
	{
		StatsComponent->DecreaseHealth(Damage);
		if (StatsComponent->GetHealth() <= 0.f)
		{
			HandleDead(HitLocation);
		}
		else
		{
			HandleHitted(HitLocation, ShotFromDirection);
		}
	}
}

void ACombatCharacter::HandleHitted(const FVector& HitLocation,const FVector& ShotFromDirection)
{
	UGameplayStatics::PlaySoundAtLocation(this, HitSound, HitLocation);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitImpact, HitLocation, FRotator());

	float DotProductValue = FVector::DotProduct(GetActorForwardVector(), ShotFromDirection);
	// FString DotProductString = FString::SanitizeFloat(DotProductValue);
	// GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Red, DotProductString);
	if (DotProductValue > 0.f)
	{
		PlayAnimMontage(HitReactMontage_Behind);
	}
	else
	{
		PlayAnimMontage(HitReactMontage_Front);
	}

		
	if (CombatComponent)
	{
		CombatComponent->SetCombatState(ECombatState::ECS_Hitted);
	}
}

void ACombatCharacter::HandleDead(const FVector& HitLocation)
{
	// neu la enemy character thi minh se an thanh mau truoc khi 
	// xu ly doan code o duoi
	UGameplayStatics::PlaySoundAtLocation(this,	DeadSound, HitLocation);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitImpact, HitLocation, FRotator());
	PlayAnimMontage(DeadMontage);

	if (CombatComponent)
	{
		CombatComponent->SetCombatState(ECombatState::ECS_Dead);
	}

	//disable collision capsule component(root component)
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// xoa luon nhan vat ra khoi map sau 1 khoang thoi gian (2 s)
	GetWorldTimerManager().SetTimer(
		DeadTimer,
		this,
		&ACombatCharacter::HandleDeadTimerFinished,
		DeadTime
	);

	// Disable Movement
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();
}

void ACombatCharacter::HandleDeadTimerFinished()
{
	Destroy();
}


void ACombatCharacter::UpdateHealth_HUD(const float& NewHealth, const float& MaxHealth)
{

}

void ACombatCharacter::UpdateEnergy_HUD(const float& NewEnergy,const float& MaxEnergy)
{

}

float ACombatCharacter::GetSpeed()
{
	Velocity = GetVelocity();
	Velocity.Z = 0.f;
	return Velocity.Size();
}

const float ACombatCharacter::GetDamageOfLastAttack()
{
	if (CombatComponent == nullptr)
	{
		return 0.0f;
	}
	return CombatComponent->GetDamageOfLastAttack();
}



void ACombatCharacter::Sprint()
{
	SpeedMode = ESpeedMode::ESM_Sprint;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void ACombatCharacter::Jog()
{
	SpeedMode = ESpeedMode::ESM_Jog;
	GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
}

bool ACombatCharacter::IsDead()
{
	if (CombatComponent == nullptr)
	{
		return false;
	}
	return CombatComponent->GetCombatState() == ECombatState::ECS_Dead;
}

const bool ACombatCharacter::IsAttacking()
{
	if (CombatComponent == nullptr)
	{
		return false;
	}
	return CombatComponent->GetCombatState() == ECombatState::ECS_Attack;
}

const bool ACombatCharacter::IsDefending()
{
	if (CombatComponent == nullptr)
	{
		return false;
	}
	return CombatComponent->GetCombatState() == ECombatState::ECS_Defend;
}







