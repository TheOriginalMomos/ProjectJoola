// By Michaël Lavoie
// Object that contains a camera location and rotation as well as a pointer
// to the next camera to use in the animation

#pragma once

#include "Engine/Engine.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraLocation.generated.h"

UCLASS()
class PROJECTJOOLA_API ACameraLocation : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACameraLocation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cameras")
		ACameraLocation* NextCamera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
