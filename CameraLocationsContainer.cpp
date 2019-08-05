
// Object used to contain the list of every camera in the scene
// as well as the interactive objects a camera can trigger when it becomes active.


#include "CameraLocationsContainer.h"


// Sets default values
ACameraLocationsContainer::ACameraLocationsContainer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ACameraLocationsContainer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACameraLocationsContainer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

