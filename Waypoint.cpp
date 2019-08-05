

#include "Waypoint.h"
#include "Monster.h"


// Sets default values
AWaypoint::AWaypoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(Root);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
	BoxComponent->SetupAttachment(GetRootComponent());
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWaypoint::OnPlayerEnter);

}

// Called when the game starts or when spawned
void AWaypoint::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWaypoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWaypoint::OnPlayerEnter(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bfromsweeep, const FHitResult &SweepResult)
{
	
	AMonster* Character = nullptr;

	if (OtherActor != nullptr)
	{
		Character = Cast<AMonster>(OtherActor);
		if (Character != nullptr)
		{
			if (Character->GetGameMode()->getdoorOpened() == 2 && !b2IsUnlocked)
			{
				UnlockNextZone(2);
			}

			if (Character->GetGameMode()->getdoorOpened() == 4 && !b3IsUnlocked)
			{
				UnlockNextZone(3);
			}

			IncStepOn++;

			Character->SetLastWaypoint(this);

			if (sizeof(NextWaypoint) == 1)
			{
				Character->NextWaypoint = NextWaypoint[0];
			}
			else
			{
				Character->NextWaypoint = RandomisePatterm();
			}


		}
	}
}

void AWaypoint::UnlockNextZone(int Zone)
{
	
	if (Zone == 2 && NextZone2Waypoint != nullptr)
	{	
		b2IsUnlocked = true;
		NextWaypoint.Add(NextZone2Waypoint);
	}
	else if (Zone == 3 && NextZone3Waypoint != nullptr)
	{
		b3IsUnlocked = true;
		NextWaypoint.Add(NextZone3Waypoint);
	}
}

AWaypoint* AWaypoint::RandomisePatterm()
{
	bool randomnumber = FMath::RandBool();
	AWaypoint* Choix = nullptr;
	uint8 Len = NextWaypoint.Num();

	for (auto i = 0; i < Len; ++i)
	{
		if (Choix == nullptr)
		{
			Choix = NextWaypoint[i];
		}
		else if (Choix->GetIncStepOn() > NextWaypoint[i]->GetIncStepOn())
		{
			Choix = NextWaypoint[i];
		}
		else if (Choix->GetIncStepOn() == NextWaypoint[i]->GetIncStepOn())
		{
			if (randomnumber)
			{
				Choix = NextWaypoint[i];
			}
		}

	}

	return Choix;

}



