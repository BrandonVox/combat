

#include "CombatCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Combat/MyComponents/CombatComponent.h"
#include "Combat/MyComponents/CollisionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
//
#include "GameFramework/PlayerController.h"
#include "Combat/HUD/CombatHUD.h"
#include "Combat/HUD/CombatWidget.h"

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

}

void ACombatCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Actions
	// Pressed
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ACombatCharacter::AttackButtonPressed);
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
}

void ACombatCharacter::BeginPlay()
{
	Super::BeginPlay();

	SpeedMode = ESpeedMode::ESM_Jog;
	GetCharacterMovement()->MaxWalkSpeed = JogSpeed;

	OnTakePointDamage.AddDynamic(this, &ACombatCharacter::OnReceivedPointDamage);

	// HUD
	APlayerController* PlayerController = Cast<APlayerController>( GetController());
	if (PlayerController)
	{
		 CombatHUD = Cast<ACombatHUD> ( PlayerController->GetHUD());
		 if (CombatHUD)
		 {
			 CombatHUD->CreateCombatWidget();
			 CombatHUD->AddWidgetToViewport(CombatHUD->GetCombatWidget());
		 }
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


void ACombatCharacter::AttackButtonPressed()
{
	CombatComponent->RequestAttack();
}

void ACombatCharacter::SprintButtonPressed()
{
	SpeedMode = ESpeedMode::ESM_Sprint;
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void ACombatCharacter::SprintButtonReleased()
{
	SpeedMode = ESpeedMode::ESM_Jog;
	GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
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




