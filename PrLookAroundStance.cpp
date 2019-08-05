

#include "PrLookAroundStance.h"

PrLookAroundStance::PrLookAroundStance(AMonster& monster)
{
	monster.GetCharacterMovement()->MaxWalkSpeed = 0;
	monster.PlayerSafety(true);
	monster.GameMode->StopMusic(10.f, 1.5f);
	monster.SetSongStarted(false);
}

PrLookAroundStance::~PrLookAroundStance()
{
}

void PrLookAroundStance::UpdateStance(AMonster& monster)
{
	if (TimeLook <= 0)
	{
		TimeLook = 300;
		MonsterStance::ChangeState(monster, new WalkStance(monster));
		monster.CharacterMovement->RotationRate = FRotator(0.0f, 1000.0f, 0.0f);
		monster.GameMode->PlayStableMusic(5.5f, 0.5f, 0.5f);
	}
	else
	{
		TimeLook--;
	}
}

