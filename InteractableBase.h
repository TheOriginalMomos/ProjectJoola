

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interactable.h"
#include "ProjectJoolaGameModeBase.h"
#include "InteractableBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTJOOLA_API UInteractableBase : public UActorComponent, public IInteractable
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractableBase();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Variables shared between all of the different Interactable components
	UPROPERTY()
		AActor* OwnerActor; // Stores a ptr to the actor that owns this custom component

	// Could add another variable for the name of the "name" that the actor that owns this custom component has. Name would be set by default per class and could be change if needed. Purpose of the name would exist only
	// for interaction feedback and if needed for a more complete inventory, but that won't be the case.

	UPROPERTY()
		AProjectJoolaGameModeBase* GameMode;

	UPROPERTY()
		EInteractionType InteractionType = EInteractionType::Default;

	UPROPERTY(EditAnywhere)
		EObjectType ObjectType = EObjectType::Default;

public:	

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Functions shared between all of the different interactable components (Comes from the Interactable Interface)

	// Interact function: Allows any component/actor that implements this interface to interact with the player.
	virtual void Interact() override;

	// Activate function : Allows interactable components to interact with eachother.
	virtual void Activation() override;

	// Reset function: Will be used later when the save/reload system is implement. Will handle the reset logic of the component's owner.
	virtual void Reset() override;

	// Interact function: Allows any component/actor that implements this interface to interact with the player.
	virtual	 EInteractionType GetInteractionType() const override;

	// Interact function: Allows any component/actor that implements this interface to interact with the player.
	virtual	 EObjectType GetObjectType() const override;
};
