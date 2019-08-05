

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Curves/CurveFloat.h"
#include "Runtime/Engine/Classes/Components/TimelineComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Sound/SoundCue.h"
#include "InteractableBase.h"
#include "InteractableDoor.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Interactions), meta = (BlueprintSpawnableComponent))
class PROJECTJOOLA_API UInteractableDoor : public UInteractableBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	UInteractableDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
		APawn* PlayerPawn; // Stores the value of the  pawn of the player

	UPROPERTY()
		USceneComponent* SoundLocation;

	// Door related infos
	UPROPERTY(EditAnywhere, Category = "Door infos")
		bool Open = false;
	UPROPERTY(EditAnywhere, Category = "Door infos")
		bool bReadyState = true;
	UPROPERTY(EditAnywhere, Category = "Door infos")
		float RotateValue = 1.0f;

	UPROPERTY()
		FVector Direction;

	// Store the struct containing all the infos related to the hit that happened
	FHitResult Hit;

	UPROPERTY(EditAnywhere, Category = "Door infos")
		bool bBroken = false; // Is the door broken?

	UPROPERTY(EditAnywhere, Category = "Door infos")
		bool bUnlocked = true;

	UPROPERTY(EditAnywhere, Category = "Door infos")
		bool bActorInteraction = true; // Stores if an actor can interact with the door or not

	// Timeline related infos

	UPROPERTY()
		FTimeline MyTimeline;

	UPROPERTY(EditAnywhere, Category = "Timeline")
		UCurveFloat* ActorCurve;

	UPROPERTY(EditAnywhere, Category = "Timeline")
		UCurveFloat* BreakCurve;

	UPROPERTY(EditAnywhere, Category = "Timeline")
		UStaticMeshComponent* ActorToRotate;

	UPROPERTY()
		float CurveFloatValue;
	UPROPERTY()
		float TimelineValue;
	UPROPERTY()
		bool bMonsterUsage = false;

	// Sound related variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		USoundCue* OpeningSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		USoundCue* ClosingSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		USoundCue* SmashingSound; // Smash door open sound

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		UAudioComponent* OpeningAudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		UAudioComponent* ClosingAudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		UAudioComponent* SmashingAudioComponent;

	void SetOpen(bool bState);

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Sound related functions
	UFUNCTION()
		void PlaySoundOpening();
	UFUNCTION()
		void PlaySoundClosing();

	UFUNCTION()
		void PlaySoundSmashing();

	UFUNCTION()
		void StopAllSounds();

	UFUNCTION()
		void SetMonsterUsage(bool usage) { bMonsterUsage = usage; };

	// Door related functions
	UFUNCTION()
		void UpdateActorRotation();

	// Interact function: Allows any component/actor that implements this interface to interact with the player.
	virtual void Interact() override;

	virtual void Activation() override;

	UFUNCTION()
		void ExecuteRotation(); // Will execute the code for the rotation. Can be called by both Interact and Activation depending on the need

	UFUNCTION()
		void SetReadyState();

	UFUNCTION()
		bool GetOpenState() { return Open; };

	UFUNCTION()
		bool GetBrokenState() { return bBroken; };

	UFUNCTION()
		void SetDirection(AActor* InteractingActor); // Determines and sets the direction the door need to open

	UFUNCTION()
		void BreakDoorOpen(); // Executed by the monster to open the door and make it uncloseable

	// Timeline functions

	// Declare our delegate function to be binded with DoorControl()
	FOnTimelineFloatStatic TimelineCallback{};

	// Declare our delegate function to be binded with SetState()
	FOnTimelineEventStatic TimelineFinishedCallback{};
	
	
};
