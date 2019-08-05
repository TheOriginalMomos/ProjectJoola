

#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundCue.h"
#include "InteractableBase.h"
#include "InteractableThrowable.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Interactions), meta = (BlueprintSpawnableComponent))
class PROJECTJOOLA_API UInteractableThrowable : public UInteractableBase
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInteractableThrowable();

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	// Stores the staticMeshComponent of the actor that owns this actor component
	UPROPERTY()
		UStaticMeshComponent* StaticMeshComponent;

	// Stores the BoxCollider of the StaticMeshComponent (Child 0 of said component)
	UPROPERTY()
		UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere)
		float MinimumVelocityToReactOnHit = 80; // Sets the minimum velocity the object needs to have to spawn sound spheres

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundCue* CollisionSound; // Smash door open sound

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		UAudioComponent* CollisionAudioComponent;

	UPROPERTY()
		bool bTouchingGround;
	UPROPERTY(EditAnywhere)
		float TimeBetweenSoundEvents = 0.2f;

	/* Handle to manage the timer */
	FTimerHandle SoundSphereTimerHandle;

	UPROPERTY()
		bool bSoundSphereTimerRunning = false;
public:

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void Interact() override;

	UFUNCTION()
		void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent * overlapComponent, AActor * otherActor, UPrimitiveComponent * otherComponent, int32 otherBodyIndex, bool bFromSweep, const FHitResult & sweepResult);

	UFUNCTION()
		void ResetTimer();
	
};
