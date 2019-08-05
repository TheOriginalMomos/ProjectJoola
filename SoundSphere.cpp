
// Actor that can collide with the AI, allowing it to detect the player's interactions with the environment

#include "SoundSphere.h"


// Sets default values
ASoundSphere::ASoundSphere()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SoundSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Soundwave"));
	RootComponent = SoundSphereComponent;
	SoundSphereComponent->InitSphereRadius(SphereRadius = 1.0f);

}

// Called when the game starts or when spawned
void ASoundSphere::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASoundSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugSphere(GetWorld(), GetSpherePos(), SphereRadius, 16, FColor::Blue, false, -1.0f, '\000', 1.0f);

	// Once sphere reaches its expected radius, delete it.
	if (SphereRadius >= TargetSphereRadius)
		Destroy();

	// Increase the radius of the sphere at each frame.
	SphereRadius += PropagationSpeed * DeltaTime;
	SoundSphereComponent->SetSphereRadius(SphereRadius, true);
}

