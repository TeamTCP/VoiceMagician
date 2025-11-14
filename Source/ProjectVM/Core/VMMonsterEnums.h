#pragma once
#include "CoreMinimal.h"
#include "VMMonsterEnums.generated.h"


//몬스터 열거형
UENUM(BlueprintType)
enum class EMonsterName : uint8
{
	None	UMETA(DisplayName = "None"),	// None
	Warrior	UMETA(DisplayName = "Warrior"),	// 전사
	Archer	UMETA(DisplayName = "Archer"),	// 궁수
	Wizard	UMETA(DisplayName = "Wizard"),	// 마법사
	MinionMelee	UMETA(DisplayName = "MinionMelee"),	// 미니언
	MinionRanged	UMETA(DisplayName = "MinionRanged"),// 미니언
	MinionSiege	UMETA(DisplayName = "MinionSiege"),	// 미니언
	MinionSuper	UMETA(DisplayName = "MinionSuper"),	// 미니언
	Boss	UMETA(DisplayName = "Boss"),	// 보스
};