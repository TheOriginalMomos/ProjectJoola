#pragma once

class AMonster;


#include "Monster.h"

class PROJECTJOOLA_API MonsterStance
{
public:
	MonsterStance();
	virtual ~MonsterStance();
	virtual void UpdateStance(AMonster& monster);
	virtual FString GetStanceName() { return StanceName; };
protected:

	void ChangeState(AMonster& monster, MonsterStance* stance);
	float Speed = 0.f;
	FString StanceName = FString(TEXT("Main"));
};
