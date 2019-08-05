
// Actor used as a reference point for an interactive actor's location and rotation.

#include "LeverSpawnPoint.h"


// Sets default values
ALeverSpawnPoint::ALeverSpawnPoint()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	// Adds a ghost mesh that is only visible in the editor.
	// Simplifies the placement of spawn points.
	LeverBaseMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeverBaseMesh"));
	LeverBaseMesh->SetupAttachment(Root);
	LeverBaseMesh->SetHiddenInGame(true);

	LeverHandleMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeverHandleMesh"));
	LeverHandleMesh->SetupAttachment(LeverBaseMesh);
	LeverHandleMesh->SetHiddenInGame(true);
}

// Called when the game starts or when spawned
void ALeverSpawnPoint::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void ALeverSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

