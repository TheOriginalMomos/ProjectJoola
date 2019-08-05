
#pragma once

#include "MonsterStance.h"

class PROJECTJOOLA_API PrSearchStance : public MonsterStance
{
public:
	PrSearchStance(AMonster& monster);
	~PrSearchStance();
	void UpdateStance(AMonster& monster);
	FString GetStanceName() { return StanceName; };
protected:
	FString StanceName = FString(TEXT("Search"));
};

