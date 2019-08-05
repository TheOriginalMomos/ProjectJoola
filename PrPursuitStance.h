#pragma once

#include "MonsterStance.h"

class PROJECTJOOLA_API PrPursuitStance : public MonsterStance
{
public:
	PrPursuitStance(AMonster& monster);
	~PrPursuitStance();
	void UpdateStance(AMonster& monster);
	FString GetStanceName() { return StanceName; };
protected:
	FString StanceName = FString(TEXT("Pursuit"));
};


