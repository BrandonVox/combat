

#include "CombatCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Combat/MyComponents/CombatComponent.h"
#include "Combat/MyComponents/CollisionComponent.h"
#include "Combat/MyComponents/StatsComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
//
#include "Combat/PlayerController/CombatPlayerController.h"


ACombatCharacter::ACombatCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	// Camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;

	// Follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

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

void ACombatCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Actions
	// Pressed
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ACombatCharacter::AttackButtonPressed);
	PlayerInputComponent->BindAction("StrongAttack", IE_Pressed, this, &ACombatCharacter::StrongAttackButtonPressed);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ACombatCharacter::SprintButtonPressed);
	// Released
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ACombatCharacter::SprintButtonReleased);
	// Axises
	PlayerInputComponent->BindAxis("MoveForward", this, &ACombatCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACombatCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ACombatCharacter::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &ACombatCharacter::Turn);
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

	SpeedMode = ESpeedMode::ESM_Jog;
	GetCharacterMovement()->MaxWalkSpeed = JogSpeed;

	OnTakePointDamage.AddDynamic(this, &ACombatCharacter::OnReceivedPointDamage);

	// HUD
	CombatPlayerController = Cast<ACombatPlayerController>( GetController());
	if (CombatPlayerController)
	{
		CombatPlayerController->CreateCombatWidget();
		CombatPlayerController->AddCombatWidgetToViewport();

		// Update HUD
		CombatPlayerController->
			UpdateHealth_HUD(StatsComponent->GetHealth(), StatsComponent->GetMaxHealth());

		CombatPlayerController->
			UpdateEnergy_HUD(StatsComponent->GetEnergy(), StatsComponent->GetMaxEnergy());
	}

}

UCombatComponent* ACombatCharacter::GetCombat_Implementation() const
{
	return CombatComponent;
}

UCollisionComponent* ACombatCharacter::GetCollision_Implementation() const
{
	return CollisionComponent;
}


// Event fired when you hit something
void ACombatCharacter::OnHitActor(const FHitResult& HitResult)
{
	// GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Hit Actor");

	AActor* HittedActor = HitResult.GetActor();
	if (HittedActor)
	{
		UGameplayStatics::ApplyPointDamage(
			HittedActor,
			20.f,
			GetActorForwardVector(),
			HitResult,
			GetController(),
			this,
			UDamageType::StaticClass()
		);
	}
}

// Event fired when someone hit you
void ACombatCharacter::OnReceivedPointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy,
	FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
	const UDamageType* DamageType, AActor* DamageCauser)
{
	// Sound USoundBase
	UGameplayStatics::PlaySoundAtLocation(this, HitSound, HitLocation);
	// Spawn blood
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitImpact, HitLocation, FRotator());
	// Play hitted animation
	// PlayAnimMontage(HitReactMontage);
	PlayAnimMontage(HitReactMontage);
	// Change combat state
	CombatComponent->SetCombatState(ECombatState::ECS_Hitted);
}



// Light Attack
void ACombatCharacter::AttackButtonPressed()
{
	if (CombatComponent)
	{
		CombatComponent->RequestAttack(EAttackType::EAT_LightAttack);
	}
}

void ACombatCharacter::StrongAttackButtonPressed()
{
	if (CombatComponent)
	{
		CombatComponent->RequestAttack(EAttackType::EAT_StrongAttack);
	}
}

void ACombatCharacter::SprintButtonPressed()
{
	if (StatsComponent && StatsComponent->GetEnergy() > 0.f)
	{
		Sprint();
	}
}

void ACombatCharacter::SprintButtonReleased()
{
	if (SpeedMode == ESpeedMode::ESM_Sprint)
	{
		Jog();
	}

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

void ACombatCharacter::UpdateHealth_HUD(const float& NewHealth, const float& MaxHealth)
{
	if (CombatPlayerController)
	{
		CombatPlayerController->UpdateHealth_HUD(NewHealth, MaxHealth);
	}
}

void ACombatCharacter::UpdateEnergy_HUD(const float& NewEnergy,const float& MaxEnergy)
{
	if (CombatPlayerController)
	{
		CombatPlayerController->UpdateEnergy_HUD(NewEnergy, MaxEnergy);
	}
}

void ACombatCharacter::MoveForward(float Value)
{
	// Yaw Pitch Roll
	const FRotator ControlRotation = Controller->GetControlRotation();
	const FRotator YawRotation = FRotator(0.f, ControlRotation.Yaw, 0.f);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void ACombatCharacter::MoveRight(float Value)
{
	const FRotator ControlRotation = Controller->GetControlRotation();
	const FRotator YawRotation = FRotator(0.f, ControlRotation.Yaw, 0.f);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void ACombatCharacter::LookUp(float Value)
{
	// Pitch
	AddControllerPitchInput(Value);
}

void ACombatCharacter::Turn(float Value)
{
	// Yaw
	AddControllerYawInput(Value);
}




