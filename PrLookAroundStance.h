
#pragma once

#include "MonsterStance.h"
#include "WalkStance.h"

class PROJECTJOOLA_API PrLookAroundStance : public MonsterStance
{
public:
	PrLookAroundStance(AMonster& monster);
	virtual ~PrLookAroundStance();
	void UpdateStance(AMonster& monster);
	FString GetStanceName() { return StanceName; };
private:

	int TimeLook = 300;
	FString StanceName = FString(TEXT("LookAround"));
};

