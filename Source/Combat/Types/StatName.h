#pragma once
UENUM(BlueprintType)
enum class EStatName : uint8
{
	ESN_Health UMETA(DisplayName = "Health"),
	ESN_Energy UMETA(DisplayName = "Energy"),
	ESN_MAX UMETA(DisplayName = "MAX")
};