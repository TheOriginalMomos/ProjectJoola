

#include "PrSearchStance.h"

PrSearchStance::PrSearchStance(AMonster& monster)
{
	monster.PlayerSafety(true);
	monster.GetCharacterMovement()->MaxWalkSpeed = monster.GetCharRunSpeed();
}

PrSearchStance::~PrSearchStance()
{
}

void PrSearchStance::UpdateStance(AMonster& monster)
{

}
