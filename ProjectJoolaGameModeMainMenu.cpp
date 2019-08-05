// 

#include "ProjectJoolaGameModeMainMenu.h"
#include "Kismet/GameplayStatics.h"
#include "PJMainMenuPawn.h"
#include "InteractableBase.h"

AProjectJoolaGameModeMainMenu::AProjectJoolaGameModeMainMenu()
{

}

void AProjectJoolaGameModeMainMenu::BeginPlay()
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraLocationsContainer::StaticClass(), Actors);

	ActorContainer = Cast<ACameraLocationsContainer>(Actors[0]);
	Cameras = ActorContainer->ListofCameras;
}

// Returns the actor associated with the current index.
ACameraLocation* AProjectJoolaGameModeMainMenu::GetCameraActor()
{
	return Cameras[CurrentCameraIndex];
}

// Get the actor associated with the index.
ACameraLocation* AProjectJoolaGameModeMainMenu::GetCameraActor(int index)
{
	if (index > Cameras.Num() - 1)
		return Cameras[0];
	return  Cameras[index];
}

// OBSOLETE. Selects a specific camera referenced by the index.
void AProjectJoolaGameModeMainMenu::SetCameraIndex(int index)
{
	CurrentCameraIndex = index;
}

// OBSOLETE. Allows to loop between every camera contained within the camera list.
int AProjectJoolaGameModeMainMenu::NextCameraIndex()
{
	if (Cameras.Num() == 0)
		return 0;

	CurrentCameraIndex = (CurrentCameraIndex + 1) % Cameras.Num();
	return CurrentCameraIndex;
}

// OBSOLETE.
int AProjectJoolaGameModeMainMenu::PrevCameraIndex()
{
	return 0;
}

