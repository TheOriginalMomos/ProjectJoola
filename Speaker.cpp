

#include "Speaker.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"


// Sets default values
ASpeaker::ASpeaker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpeakerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Speaker"));
	RootComponent = SpeakerMesh;

	SpeakerAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Speaker Audio"));
	SpeakerAudioComponent->bAutoActivate = false;
	SpeakerAudioComponent->AttachTo(RootComponent);

}

// Called when the game starts or when spawned
void ASpeaker::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpeaker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpeaker::SetSpeakerSound(USoundCue *ReceivedSoundCue)
{
	if (SpeakerAudioComponent->IsValidLowLevelFast())
	{
		SpeakerAudioComponent->SetSound(ReceivedSoundCue);
	}
}

void ASpeaker::PlaySpeakerSound()
{
	if (!SpeakerAudioComponent->IsPlaying())
	{
		SpeakerAudioComponent->Play();
	}
}

