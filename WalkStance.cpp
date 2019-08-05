

#include "WalkStance.h"

WalkStance::WalkStance(AMonster& monster)
{
		monster.PlayerSafety(true);
		monster.InMind = false;
		monster.IsLooking = false;
		monster.GetCharacterMovement()->MaxWalkSpeed = monster.GetCharWalkSpeed();
}

WalkStance::~WalkStance()
{
}

void WalkStance::UpdateStance(AMonster& monster)
{

}
