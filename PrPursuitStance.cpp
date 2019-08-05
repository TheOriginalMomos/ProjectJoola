

#include "PrPursuitStance.h"

PrPursuitStance::PrPursuitStance(AMonster& monster)
{
	monster.PlayerSafety(false);
	if (!monster.GetSongStarted())
	{
		monster.GameMode->PlayPursuitMusic(1.5f,0.5f,0.5f);
		monster.Scream();
	}
	monster.SetSongStarted(true);
	monster.GetCharacterMovement()->MaxWalkSpeed = monster.GetCharRunSpeed();
	
}

PrPursuitStance::~PrPursuitStance()
{

}

void PrPursuitStance::UpdateStance(AMonster& monster)
{

}
