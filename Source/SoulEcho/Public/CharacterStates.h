#pragma once

UENUM(BlueprintType)
enum class ECharacterStates : uint8
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_1HEquipped UMETA(DisplayName = "Equipping one-hand weapon"),
	ECS_2HEquipped UMETA(DisplayName = "Equipping two-hand weapon"),
	ECS_Disarm UMETA(DisplayName = "Disarm")
};

UENUM(BlueprintType)
enum class ECombatStates : uint8
{
	ECS_Free UMETA(DisplayName = "Out of Combat"),
	ECS_Attacking UMETA(DisplayName = "Attacking")
};