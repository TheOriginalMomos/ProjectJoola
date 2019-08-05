

#include "InteractableBase.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"


// Sets default values for this component's properties
UInteractableBase::UInteractableBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractableBase::BeginPlay()
{
	Super::BeginPlay();

	OwnerActor = Cast<AActor>(GetOwner());

	GameMode = (AProjectJoolaGameModeBase*)GetWorld()->GetAuthGameMode();

	UE_LOG(LogTemp, Log, TEXT("%s owns an InteractableComponent"), *OwnerActor->GetName());
	
}


// Called every frame
void UInteractableBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInteractableBase::Interact()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("InteractableBase Interact function!"));
}

void UInteractableBase::Activation()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("InteractableBase Activation function!"));
}

void UInteractableBase::Reset()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("InteractableBase Reset function!"));
}

EInteractionType UInteractableBase::GetInteractionType() const
{
	return InteractionType;
}

EObjectType UInteractableBase::GetObjectType() const
{
	return ObjectType;
}

