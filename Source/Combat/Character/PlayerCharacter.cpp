

#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "Combat/MyComponents/StatsComponent.h"
#include "Combat/MyComponents/FocusComponent.h"
#include "Combat/MyComponents/CombatComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Combat/PlayerController/CombatPlayerController.h"
#include "CombatAnimInstance.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"



APlayerCharacter::APlayerCharacter()
{
	// Camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;

	// Follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	FocusComponent = CreateDefaultSubobject<UFocusComponent>(TEXT("FocusComponent"));

}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	ACharacter::SetupPlayerInputComponent(PlayerInputComponent);
	// Actions
	// Pressed
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::AttackButtonPressed);
	PlayerInputComponent->BindAction("StrongAttack", IE_Pressed, this, &APlayerCharacter::StrongAttackButtonPressed);
	PlayerInputComponent->BindAction("ChargeAttack", IE_Pressed, this, &APlayerCharacter::ChargeAttackButtonPressed);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacter::SprintButtonPressed);
	PlayerInputComponent->BindAction("Focus", IE_Pressed, this, &APlayerCharacter::FocusButtonPressed);
	PlayerInputComponent->BindAction("Defend", IE_Pressed, this, &APlayerCharacter::DefendButtonPressed);
	// Released
	PlayerInputComponent->BindAction("ChargeAttack", IE_Released, this, &APlayerCharacter::ChargeAttackButtonReleased);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacter::SprintButtonReleased);
	// Axises
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::Turn);
}

void APlayerCharacter::PostInitializeComponents()
{
	// Super == Combat Character
	Super::PostInitializeComponents();
	if (FocusComponent)
	{
		FocusComponent->SetPlayerCharacter(this);
	}
}

bool APlayerCharacter::HasEnoughEnergyForThisAttackType(const EAttackType& AttackType)
{
	if (StatsComponent == nullptr)
	{
		return false;
	}
	return StatsComponent->HasEnoughEnergyForThisAttackType(AttackType);
}

void APlayerCharacter::DecreaseEnergyByAttackType(const EAttackType& AttackType)
{
	if (StatsComponent)
	{
		StatsComponent->DecreaseEnergyByAttackType(AttackType);
	}
}

bool APlayerCharacter::HasEnoughEnergyForDefend()
{
	if (StatsComponent == nullptr)
	{
		return false;
	}
	return StatsComponent->GetEnergy() >= StatsComponent->GetEnergyCost_Defend();
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Set Player Controller
	CombatPlayerController = Cast<ACombatPlayerController>(GetController());
	if (CombatPlayerController)
	{
		CombatPlayerController->CreateCombatWidget();
		CombatPlayerController->AddCombatWidgetToViewport();

		UpdateHealth_HUD(StatsComponent->GetHealth(), StatsComponent->GetMaxHealth());
		UpdateEnergy_HUD(StatsComponent->GetEnergy(), StatsComponent->GetMaxEnergy());
	}
}

const FVector APlayerCharacter::GetCameraDirection()
{
	if (FollowCamera == nullptr)
	{
		return FVector();
	}
	return FollowCamera->GetForwardVector();
}


void APlayerCharacter::UpdateHealth_HUD(const float& NewHealth, const float& MaxHealth)
{
	Super::UpdateHealth_HUD(NewHealth, MaxHealth);

	if (CombatPlayerController)
	{
		CombatPlayerController->UpdateHealth_HUD(NewHealth, MaxHealth);
	}
	// neu la ke dich thi update mau o tren dau luon
}

void APlayerCharacter::UpdateEnergy_HUD(const float& NewEnergy, const float& MaxEnergy)
{
	Super::UpdateEnergy_HUD(NewEnergy, MaxEnergy);

	if (CombatPlayerController)
	{
		CombatPlayerController->UpdateEnergy_HUD(NewEnergy, MaxEnergy);
	}
}



// Focusing
void APlayerCharacter::SetControllerRotation(const FRotator& NewControllerRotation)
{
	if (Controller)
	{
		Controller->SetControlRotation(NewControllerRotation);
	}
}

void APlayerCharacter::SetupFocus(const bool& bDoFocus)
{
	if (GetMesh() == nullptr)
	{
		return;
	}

	UCombatAnimInstance* CAI = Cast<UCombatAnimInstance>(GetMesh()->GetAnimInstance());

	if (CAI)
	{
		CAI->SetIsFocusing(bDoFocus);
	}

	if (bDoFocus)
	{
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;

	}
	else
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

void APlayerCharacter::OnReceivedPointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
	if (IsDefending())
	{
		if (StatsComponent)
		{
			StatsComponent->DecreaseEnergy(StatsComponent->GetEnergyCost_Defend());
		}
		UGameplayStatics::PlaySoundAtLocation(this, DefendSound, HitLocation);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DefendHitImpact, HitLocation, FRotator());
		return;
	}

	Super::OnReceivedPointDamage(DamagedActor, Damage, InstigatedBy,
		HitLocation, FHitComponent, BoneName, ShotFromDirection, DamageType, DamageCauser);
}

// Light Attack
void APlayerCharacter::AttackButtonPressed()
{
	if (CombatComponent)
	{
		if (IsSprinting() == false)
		{
			CombatComponent->RequestAttack(EAttackType::EAT_LightAttack);
		}
		else
		{
			CombatComponent->RequestAttack(EAttackType::EAT_SprintAttack);
		}
	}
}

void APlayerCharacter::StrongAttackButtonPressed()
{
	if (CombatComponent)
	{
		CombatComponent->RequestAttack(EAttackType::EAT_StrongAttack);
	}
}

void APlayerCharacter::ChargeAttackButtonPressed()
{
	if (CombatComponent
		&& CombatComponent->GetCombatState() == ECombatState::ECS_Attack)
	{
		UCombatAnimInstance* CAI = Cast<UCombatAnimInstance>(GetMesh()->GetAnimInstance());
		if (CAI && CAI->IsAnyMontagePlaying())
		{
			CAI->StopAllMontages(0.3f);
			CombatComponent->ResetCombat();
		}
	}


	// chay timer, doi 1 khoang thoi gian sau do cho nhan vat charge attack
	// chay timer
	GetWorldTimerManager().SetTimer(
		ChargeAttackTimer,
		this,
		&APlayerCharacter::HandleChargeTimerFinish,
		ChargeTime
	);
	// sau khi timer chay xong thi minh moi attack
}

void APlayerCharacter::HandleChargeTimerFinish()
{
	if (CombatComponent)
	{
		CombatComponent->RequestAttack(EAttackType::EAT_ChargeAttack);
	}
}

void APlayerCharacter::HandleDead(const FVector& HitLocation)
{
	Super::HandleDead(HitLocation);
	if (CombatPlayerController)
	{
		CombatPlayerController->ShowDefeatWidget();
	}
}

void APlayerCharacter::ChargeAttackButtonReleased()
{
	if (GetWorldTimerManager().IsTimerActive(ChargeAttackTimer))
	{
		GetWorldTimerManager().ClearTimer(ChargeAttackTimer);
	}
}

void APlayerCharacter::SprintButtonPressed()
{
	// kiem tra co dang focus hay khong
	// neu co thi return
	if (FocusComponent && FocusComponent->IsFocusing())
	{
		return;
	}



	if (StatsComponent && StatsComponent->GetEnergy() > 0.f)
	{
		Sprint();
	}
}

void APlayerCharacter::FocusButtonPressed()
{
	if (FocusComponent == nullptr)
	{
		return;
	}

	if (FocusComponent->IsFocusing() == false)
	{
		FocusComponent->Focus();
	}
	else
	{
		FocusComponent->UnFocus();
	}
}

void APlayerCharacter::DefendButtonPressed()
{
	if (CombatComponent)
	{
		CombatComponent->RequestDefend();
	}
}

void APlayerCharacter::SprintButtonReleased()
{
	if (SpeedMode == ESpeedMode::ESM_Sprint)
	{
		Jog();
	}

}


void APlayerCharacter::MoveForward(float Value)
{
	// Yaw Pitch Roll
	const FRotator ControlRotation = Controller->GetControlRotation();
	const FRotator YawRotation = FRotator(0.f, ControlRotation.Yaw, 0.f);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void APlayerCharacter::MoveRight(float Value)
{
	const FRotator ControlRotation = Controller->GetControlRotation();
	const FRotator YawRotation = FRotator(0.f, ControlRotation.Yaw, 0.f);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void APlayerCharacter::LookUp(float Value)
{
	// Pitch
	AddControllerPitchInput(Value);
}

void APlayerCharacter::Turn(float Value)
{
	// Yaw
	AddControllerYawInput(Value);
}

