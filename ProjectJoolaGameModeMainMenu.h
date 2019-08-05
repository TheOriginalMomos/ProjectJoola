//

#pragma once

#include "Map.h"
#include "GameFramework/Actor.h"
#include "CameraLocation.h"
#include "CameraLocationsContainer.h"
#include "Engine/World.h"

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectJoolaGameModeMainMenu.generated.h"

/**
 *
 */
UCLASS()
class PROJECTJOOLA_API AProjectJoolaGameModeMainMenu : public AGameModeBase
{
	GENERATED_BODY()

public:

	AProjectJoolaGameModeMainMenu();

	virtual void BeginPlay() override;

	int GetCameraIndex() { return CurrentCameraIndex; }

	ACameraLocation* GetCameraActor();

	ACameraLocation* GetCameraActor(int index);

	void SetCameraIndex(int index);

	int NextCameraIndex();

	int PrevCameraIndex();

	ACameraLocationsContainer* ActorContainer;

	TArray<ACameraLocation*> Cameras;

private:
	UPROPERTY(EditAnywhere, Category = "Cameras")
		int CurrentCameraIndex = 0;

};
