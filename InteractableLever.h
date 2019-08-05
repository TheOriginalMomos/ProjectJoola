

#pragma once

#include "CoreMinimal.h"
#include "InteractableBase.h"
#include "Sound/SoundCue.h"
#include "ProjectJoolaGameModeBase.h"
#include "InteractableLever.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Interactions), meta = (BlueprintSpawnableComponent))
class PROJECTJOOLA_API UInteractableLever : public UInteractableBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	UInteractableLever();

	UPROPERTY(EditAnywhere)
		// Stores the actors that this button has to activate the interact function
		TArray<AActor*> ActorsToInteract;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Variables unique to levers

	UPROPERTY(BlueprintReadWrite)
		//Determines if the lever has been activated or not.
		bool bActivated = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		//Determines if there's an animation playing or not.
		bool bAnimationIsPlaying = false;

public:

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Sound related variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundCue* DownSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundCue* UpSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		UAudioComponent* DownAudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		UAudioComponent* UpAudioComponent;

	// Sound related functions
	UFUNCTION()
		void PlaySoundDown();
	UFUNCTION()
		void PlaySoundUp();

	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Interact function: Allows any component/actor that implements this interface to interact with the player.
	virtual void Interact() override;

	// Reset function: Will be used later when the save/reload system is implement. Will handle the reset logic of the component's owner.
	//virtual void Reset() override;

	UFUNCTION()
		bool GetbActivated() { return bActivated; };
};
