#pragma once
UENUM(BlueprintType)
enum class EAttackType : uint8
{
	EAT_LightAttack UMETA(DisplayName = "LightAttack"),
	EAT_StrongAttack UMETA(DisplayName = "StrongAttack"),
	EAT_SprintAttack UMETA(DisplayName = "SprintAttack"),
	EAT_ChargeAttack UMETA(DisplayName = "ChargeAttack"),
	EAT_MAX UMETA(DisplayName = "MAX")
};