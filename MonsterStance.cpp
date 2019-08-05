
#include "MonsterStance.h"



void MonsterStance::UpdateStance(AMonster& monster)
{

}

MonsterStance::~MonsterStance()
{

}

MonsterStance::MonsterStance()
{

}

void MonsterStance::ChangeState(AMonster& monster, MonsterStance* thisStance)
{
	MonsterStance* oldState = monster.CurrentStance;
	monster.CurrentStance = thisStance;
	delete oldState;
}
