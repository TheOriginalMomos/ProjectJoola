

#pragma once

#include "CoreMinimal.h"
#include "InteractableLever.h"
#include "InteractableSwitch.h"
#include "InteractableDoor.h"
#include "InteractableDoorLocked.generated.h"


UENUM()
enum class ELockType : uint8
{
	KeyLock UMETA(Displayname = "Key"),
	LeverLock UMETA(Displayname = "Lever")

};

/**
 * 
 */
UCLASS(ClassGroup = (Interactions), meta = (BlueprintSpawnableComponent))
class PROJECTJOOLA_API UInteractableDoorLocked : public UInteractableDoor
{
	GENERATED_BODY()
	
public:

	UInteractableDoorLocked();

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	// Variables unique to LeverLock type

	UPROPERTY(EditAnywhere, Category = "Light infos")
		// Stores the actors this component has to update when the door is unlocked with levers
		TArray<AActor*> LightActorsToUpdate;

	UPROPERTY(EditAnywhere, Category = "Lock variables")
		ELockType LockType;

	UPROPERTY(EditAnywhere, Category = "Lock variables")
		int NbOfKeysRequired;

public:

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Interact function: Allows any component/actor that implements this interface to be called for interaction.
	virtual void Interact() override;

	virtual void Activation() override;

	// Might just create public functions to set these two instead of making them public
	UPROPERTY()
		// Stores only the InteractableComponents of the actors that actually have it
		TArray<UInteractableSwitch*> InteractableLeversToCheck;

	UPROPERTY(EditAnywhere, Category = "Lock variables")
		// Stores the number of Activated Actors it needs to open
		int RequiredNumberOfActivatedActors;

	// Functions unique to InteractableRotationUnlockable

	UFUNCTION()
		void CheckKeysRequirement(int PlayerKeys);

	UFUNCTION()
		// Checks if the requirements to Unlock the actor have been met. Will be called everytime a lever gets activated.
		void CheckLeversRequirement();
	
	UFUNCTION()
		ELockType GetLockType() { return LockType; };

	UFUNCTION()
		void PlayUnlockSound();

	UFUNCTION()
		void UpdateLightColor();
};
