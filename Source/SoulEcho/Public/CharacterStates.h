#pragma once

UENUM(BlueprintType)
enum ECharacterStates : uint8
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_1HEquipped UMETA(DisplayName = "Equipping one-hand weapon"),
	ECS_2HEquipped UMETA(DisplayName = "Equipping two-hand weapon"),
	ECS_Disarm UMETA(DisplayName = "Disarm")
};
