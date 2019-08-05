
// Actor that can collide with the AI, allowing it to detect the player's interactions with the environment

#pragma once

#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SoundSphere.generated.h"

UCLASS()
class PROJECTJOOLA_API ASoundSphere : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASoundSphere();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		USphereComponent* SoundSphereComponent;

	float TargetSphereRadius;
	float PropagationSpeed = 20.0f;

private:
	float SphereRadius;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FVector GetSpherePos() { return this->GetActorLocation(); };
};
