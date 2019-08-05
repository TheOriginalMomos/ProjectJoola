

#include "PrLookSound.h"

PrLookSound::PrLookSound(AMonster& monster)
{
	monster.GetCharacterMovement()->MaxWalkSpeed = 0;
	monster.CharacterMovement->RotationRate = FRotator(0.0f, 0.0f, 0.0f);
}

PrLookSound::~PrLookSound()
{
}

void PrLookSound::UpdateStance(AMonster& monster)
{
	if (TimeLook <= 0)
	{
		TimeLook = 200;
		monster.CharacterMovement->RotationRate = FRotator(0.0f, 1000.0f, 0.0f);
		MonsterStance::ChangeState(monster, new WalkStance(monster));
	}
	else
	{
		TimeLook--;
	}
}