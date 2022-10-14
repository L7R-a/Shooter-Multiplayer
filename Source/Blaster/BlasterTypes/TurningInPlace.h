#pragma once

UENUM(BlueprintType)
enum class ETurningInPlace : uint8
{
	ETIP_Left UMETA(DiplayName = "Turning Left"),
	ETIP_Right UMETA(DiplayName = "Turning Right"),
	ETIP_NotTurning UMETA(DiplayName = "Not Turning"),

	ETIP_MAX UMETA(DiplayName = "DefaultMax")
};