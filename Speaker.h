

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "Components/StaticMeshComponent.h"
#include "Speaker.generated.h"

UCLASS()
class PROJECTJOOLA_API ASpeaker : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpeaker();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Speaker properties")
		UStaticMeshComponent* SpeakerMesh;
	UPROPERTY(EditAnywhere, Category = "Speaker properties")
		USoundCue* SpeakerSoundCue;
	UPROPERTY(EditAnywhere, Category = "Speaker properties")
		UAudioComponent* SpeakerAudioComponent;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetSpeakerSound(USoundCue*); // Sets the SpeakSoundComponent with whatever soundcue received. For now the inital one will be called in GameModeBase.cpp

	void PlaySpeakerSound(); // Play the SpeakSoundComponent

	
	
};
