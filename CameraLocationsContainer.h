
// Object used to contain the list of every camera in the scene
// as well as the interactive objects a camera can trigger when it becomes active.

#pragma once

#include "CameraLocation.h"
#include "InteractableBase.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraLocationsContainer.generated.h"

UCLASS()
class PROJECTJOOLA_API ACameraLocationsContainer : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACameraLocationsContainer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cameras")
		TArray<ACameraLocation*> ListofCameras;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cameras")
		TArray<AActor*> InteractablesToTrigger;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;



};
