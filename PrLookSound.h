
#pragma once

#include "MonsterStance.h"
#include "WalkStance.h"

class PROJECTJOOLA_API PrLookSound : public MonsterStance
{
public:
	PrLookSound(AMonster& monster);
	virtual ~PrLookSound();
	void UpdateStance(AMonster& monster);
	FString GetStanceName() { return StanceName; };
private:
	int TimeLook = 200;
	FString StanceName = FString(TEXT("LookAroundHearing"));
};
