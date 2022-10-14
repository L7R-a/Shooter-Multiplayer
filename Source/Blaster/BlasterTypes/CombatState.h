#pragma once

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	ECS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ECS_Reloading UMETA(DisplayName = "Reloading"),
	ECS_ThrowingGranade UMETA(DisplayName = "Throwing Granade"),
	ECS_SwappingWeapons UMETA(DisplayName = "Swapping Weapons"),
	ECS_MAX UMETA(DisplayName = "DefaultMAX")
};