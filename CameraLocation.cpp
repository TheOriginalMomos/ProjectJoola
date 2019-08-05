// By Michaël Lavoie
// Object that contains a camera location and rotation as well as a pointer
// to the next camera to use in the animation

#include "CameraLocation.h"


// Sets default values
ACameraLocation::ACameraLocation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ACameraLocation::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACameraLocation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

