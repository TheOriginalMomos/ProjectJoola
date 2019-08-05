

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableBase.h"
#include "InteractableDoorLocked.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Sound/SoundCue.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "PlayerHUD.h"
#include "Interaction.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTJOOLA_API UInteraction : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInteraction();

private:
	UPROPERTY()
		ACharacter * OwnerCharacter; // Stores the character that owns the Interaction component
	UPROPERTY()
		UInteractableBase* InteractableComponent; // Stores the Interactable Component of the currentInteractableActor
	UPROPERTY()
		APlayerHUD* PlayerHUD;
	UPROPERTY()
		UPhysicsHandleComponent* OwnerPhysicsHandle; // Stores the owner's PhysicsHandle

	UPROPERTY(EditAnywhere)
		USceneComponent* HoldLocation;
	UPROPERTY()
		AActor* ActorToGrabRelease; // Stores the Component to grab/release
	UPROPERTY()
		UPrimitiveComponent* ComponentToGrabRelease; // Stores the Component to grab/release
	UPROPERTY()
		UInteractableDoorLocked* InteractableDoorLockedComponent; // Stores the Interactable Component of the currentInteractableActor when it's a locked door. Couldn't think of another way to get around the problem
	UPROPERTY()
		EInteractionType InteractableType = EInteractionType::NoType; // Stores the Interacable Component's interaction type
	UPROPERTY()
		EObjectType ObjectType = EObjectType::Default; // Stores the Interacable Component's interaction type

	UPROPERTY(EditAnywhere, Category = "Interactions")
		float InteractionDistance = 250.0f; // Maximum distance possible for interactions in cm

	UPROPERTY(EditAnywhere, Category = "Interactions")
		float ThrowStrength = 1250.0f; // Determines the strength of the throw action.

	UPROPERTY()
		bool bHoldingObject = false;


	UPROPERTY(EditAnywhere)
		class UCameraComponent* OwnerMainCamera; // Stores the main camera component of the owner of the Interaction component

	UPROPERTY(EditAnywhere)
		bool bDrawTrace = false;
	// Stores th Old Interactable Actor
	// Used to check if the current Interactable Actor is the same as the one previously detected last time it detected a collision
	AActor* InteractableActorOld;
	// Stores the current possible Interactable Actor
	// Used to check if the current Interactable Actor is the same as the one previously detected last time it detected a collision
	AActor* InteractableActorCurrent;

	// Store the struct containing all the infos related to the hit that happened
	FHitResult Hit;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Contains the code to raycast and store the informations of what it hits
	void CheckForInteractable();

	// Main function of the Interaction's component
	// Checks the type of interaction it's dealing with and calls the appropriate function
	void Interaction();

	void Grab(); // Grabs throwable objects (Could be modified if the needs were there, but grabbing throwable objects is the only function needed for this game)

	void Release(); // Release the grabbed object without throwing it. Will produce less sound than throwing the object

	void Throw(); // Throws the grabbed object in front of the player. Will produce more sound per impact.

	void UpdateGrabbedActorLocationAndRotation(); // Called every frame to update the location and rotation of the Grabbed Actor

	// Might have to add a Grab function to grab the throwable objects

	// Might have to add a Release function to release the throwable objects

	// Might have to add a Throw function to throw the throwable object forward

};
