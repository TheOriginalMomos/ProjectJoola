
// Note trying to name a function "Activate" doesn't work and resulted in errors betwen InteractableBase and Interactable where InteractableBase didn't have the same number of parameters than Interactable despite both having 0. So I called the function Activation instead
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

UENUM()
enum class EInteractionType : uint8
{
	Default UMETA(DisplayName = "Default"),

	Grabbable UMETA(Displayname = "Grabbable"),

	Throwable UMETA(Displayname = "Throwable"),

	Unlockable UMETA(Displayname = "Unlockable"),

	NoType UMETA(Displayname = "NoType")
};

UENUM()
enum class EObjectType : uint8
{
	Default UMETA(DisplayName = "Object"),

	Door UMETA(Displayname = "Door"),

	Lever UMETA(Displayname = "Lever"),

	Switch UMETA(Displayname = "Switch"),

	Key UMETA(Displayname = "Key")
};

/**
 * 
 */
class PROJECTJOOLA_API IInteractable
{
	GENERATED_BODY()

		// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// Interact function: Allows any component/actor that implements this interface to interact with the player.
	UFUNCTION(Category = "Interaction")
		virtual void Interact() = 0;

	// Activation function : Allows custom components to communicate to other components without having to interact with the player.
	UFUNCTION(Category = "Interaction")
		virtual void Activation() = 0;

	// Reset function: Will be used later when the save/reload system is implement. Will handle the reset logic of the component's owner.
	UFUNCTION(Category = "Interaction")
		virtual void Reset() = 0;

	// Interact function: Allows any component/actor that implements this interface to interact with the player.
	UFUNCTION(Category = "Interaction")
		virtual EInteractionType GetInteractionType() const = 0;

	// Interact function: Allows any component/actor that implements this interface to interact with the player.
	UFUNCTION(Category = "Interaction")
		virtual EObjectType GetObjectType() const = 0;
};
