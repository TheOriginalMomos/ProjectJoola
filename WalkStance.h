#pragma once

#include "MonsterStance.h"

class PROJECTJOOLA_API WalkStance : public MonsterStance
{
public:
	WalkStance(AMonster& monstre);
	~WalkStance();
	void UpdateStance(AMonster& monster);
	FString GetStanceName() { return StanceName; };
protected:
	FString StanceName = FString(TEXT("Walk"));
};
