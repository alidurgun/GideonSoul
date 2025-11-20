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
enum class EActorState : uint8
{
	EAS_Alive UMETA(DisplayName = "Alive"),
	EAS_Dead UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class ECombatStates : uint8
{
	ECS_Free UMETA(DisplayName = "Out of Combat"),
	ECS_GetHit UMETA(DisplayName = "GetHit"),
	ECS_Attacking UMETA(DisplayName = "Attacking"),
	ECS_Patrolling UMETA(DisplayName = "Patrolling"),
	ECS_Chasing UMETA(DisplayName = "Chasing"),
	ECS_Engaged UMETA(DisplayName = "Engaged")
};