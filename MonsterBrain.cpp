

#include "MonsterBrain.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AMonsterBrain::AMonsterBrain()
{
	PrimaryActorTick.bCanEverTick = true;

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AISightAngle;
	SightConfig->SetMaxAge(AISightAge);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AMonsterBrain::OnPawnDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);

	
}

void AMonsterBrain::BeginPlay()
{
	Super::BeginPlay();

}

void AMonsterBrain::Possess(APawn* Pawn)
{
	Super::Possess(Pawn);
	Monster = Cast<AMonster>(Pawn);
}

void AMonsterBrain::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	GetMonsterPosition();
	MonsterMechanics();

}

FRotator AMonsterBrain::GetControlRotation() const
{
	if (GetPawn() == nullptr)
	{
		return FRotator(0.0f, 0.0f, 0.0f);
	}

	return FRotator(0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f);

}


void AMonsterBrain::OnPawnDetected(const TArray<AActor*> &DetectedPawns)
{
	for (size_t i = 0; i < DetectedPawns.Num(); i++)
	{
		DistanceTarget = GetPawn()->GetDistanceTo(DetectedPawns[i]);
	}

	Player = Cast<APJCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (DistanceTarget <= AISightRadius)
		PlayerInCircle = true;
	else
		PlayerInCircle = false;

	if (!PlayerInSight)
		PlayerInSight = true;
	else
		PlayerInSight = false;
}

void AMonsterBrain::CheckifVisible()
{

	DistanceTarget = Monster->GetDistanceTo(Player);

	if ((Player->GetVisibility() >= DistanceTarget && PlayerInSight) || CheckForLight())
	{
		Monster->SetInSight(true);
		Monster->SetInMind(true);
	}
	else
	{
		if (Monster->GetInSight())
		PlayerLastSeen = Player->GetActorLocation();

		Monster->SetInSight(false);
		
	}

	
}

void AMonsterBrain::MonsterMechanics() //ARevoir
{
	if (PlayerInCircle)
		CheckifVisible();


	if (DistanceTarget < 150.f)
		Monster->SetCharacterTooclose(true);
	else 
		Monster->SetCharacterTooclose(false);

	if (Monster->NextWaypoint != nullptr)
	{
		if (!Monster->GetInMind() && !Monster->GetInSight())
		{
			if (Monster->GetIsHearing())
			{
				if (FVector::Dist(Monster->GetActorLocation(), Monster->GetPosLastHeard()) <= 250.f )
				{
					Monster->SetIsHearing(false);
				}
				MoveToLocation(Monster->GetPosLastHeard(), 150.f);
			}
			else
			{
				
				MoveToActor(Monster->NextWaypoint, 5.0f);
			}
			
		}
		else if (Monster->GetInMind() && Monster->GetInSight())
		{
			MoveToActor(Player, 0.0f);

		}
		else if (Monster->GetInMind() && !Monster->GetInSight())
		{
			if (FVector::Dist(Monster->GetActorLocation(), PlayerLastSeen) <= 100.f || !PlayerInCircle)
			{
				Monster->SetIsLookingTrue();
				
			}
			else
			{
				MoveToLocation(PlayerLastSeen, 10.0f);
			}	
			
		}
	}

	
		
}

bool AMonsterBrain::CheckForLight()
{
	bool vision = false;
	TArray<ALightVision*> FoundActors;
	FindAllActors(GetWorld(), FoundActors);
	
	for (auto i=0;i < FoundActors.Num();i++)
	{
		if (!vision && FVector::Dist(Monster->GetActorLocation(), FoundActors[i]->GetActorLocation()) <= 500.f)
		{
			vision = LineOfSightTo(FoundActors[i]);
		}
	}

	if (!Player->GetPlayerInvisibility())
		return vision;
	else
		return false;
}


template<typename T>
void AMonsterBrain::FindAllActors(UWorld* World, TArray<T*>& Out) //GetAllActorForClass
{
	for (TActorIterator<T> It(World); It; ++It)
	{
		Out.Add(*It);
	}
}

void AMonsterBrain::GetMonsterPosition()
{

	if (CurrentPosition != Monster->GetActorLocation())
	{
		GetWorldTimerManager().SetTimer(DebugTickTimerHandle, this, &AMonsterBrain::ResetMonster, DebugDecayTickRate, true);
	}

	CurrentPosition = Monster->GetActorLocation();
}

void AMonsterBrain::ResetMonster()
{
	if (Monster)
	{
		Monster->SetInMind(false);
		Monster->SetInSight(false);
		Monster->SetIsHearing(false);
		Monster->SetIsLookingTrue();
	}
}

