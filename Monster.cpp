

#include "Monster.h"
#include "PrSearchStance.h"
#include "PrPursuitStance.h" //please pray for me for forgiveness
#include "WalkStance.h"
#include "PrLookAroundStance.h"
#include "PrLookSound.h"
#include "MonsterStance.h"
#include "Components/BoxComponent.h"


AMonster::AMonster()
{
	PrimaryActorTick.bCanEverTick = true;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.f, 0.0f);
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();
	GameMode = (AProjectJoolaGameModeBase*)GetWorld()->GetAuthGameMode();

	CurrentStance = new WalkStance(*this);
	GameMode->PlayStableMusic(0.5f, 0.5f, 0.5f);
	
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurrentStance->UpdateStance(*this);
}

void AMonster::SetInMind(bool IsInMind)
{
	if (IsInMind != InMind)
	{
		InMind = IsInMind;
		ChangeStance();
	}
}

void AMonster::SetIsLookingTrue()
{
	if (!IsLooking)
	{
		IsLooking = true; 
		ChangeStance();
	}
	
}

void AMonster::SetIsHearing(bool IsInHearing)
{
		IsHearing = IsInHearing;
		if (!IsInHearing && CurrentStance->GetStanceName() == TEXT("Walk"))
		{
			CurrentStance = new PrLookSound(*this);
		}
}



void AMonster::SetInSight(bool IsInSight)
{
	if (IsInSight != InSight)
	{
		InSight = IsInSight;
		ChangeStance();
	}
}


// Called to bind functionality to input
void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMonster::ChangeStance()//A changer vers Prisoner
{
	if (InSight && InMind)
	{
		CurrentStance = new PrPursuitStance(*this);
	}
	else if (!InSight && !InMind)
	{
		if (IsLooking)
		{
			CurrentStance = new PrLookAroundStance(*this);
		}
		else
		{
			CurrentStance = new WalkStance(*this);
		}
	}
	else if (!InSight && InMind)
	{
		if (IsLooking)
		{
			CurrentStance = new PrLookAroundStance(*this);
		}
		else
		{ 
			CurrentStance = new PrSearchStance(*this);
		}

	}
}

void AMonster::Scream()
{

}

void AMonster::PlayerSafety(bool state)
{

}

