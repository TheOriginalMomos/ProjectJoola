// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectJoolaGameModeBase.h"
#include "InteractableDoorLocked.h"
#include "Components/AudioComponent.h"
#include "InteractableSwitch.h"
#include "Engine/Engine.h"

AProjectJoolaGameModeBase::AProjectJoolaGameModeBase()
{
	/*AlarmAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Alarm"));
	AlarmAudioComponent->bAutoActivate = false;*/
	
	StableMusicAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("StableMusic"));
	StableMusicAudioComponent->bAutoActivate = true;

	PursuitMusicAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("PursuitMusic"));
	PursuitMusicAudioComponent->bAutoActivate = false;
}

void AProjectJoolaGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, (TEXT("GameMode")));

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpeaker::StaticClass(), Speakers);

	for (int i = 0; i < Speakers.Num(); i++)
	{
		Cast<ASpeaker>(Speakers[i])->SetSpeakerSound(AlarmSound);
	}

	if (StableMusicAudioComponent->IsValidLowLevelFast()) {
		StableMusicAudioComponent->SetSound(StableMusic);
	}
	if (PursuitMusicAudioComponent->IsValidLowLevelFast()) {
		PursuitMusicAudioComponent->SetSound(PursuitMusic);
	}

	PlaceLeversInLevel();
	PlayStableMusic(0.f, 0.f, 0.f);
}

void AProjectJoolaGameModeBase::InstantiateSoundWaveSphere(FVector loc, float radius)
{
	ASoundSphere* SoundSphere = GetWorld()->SpawnActor<ASoundSphere>(loc, FRotator::ZeroRotator, FActorSpawnParameters());
	SoundSphere->TargetSphereRadius = radius;
	SoundSphere->PropagationSpeed = PropagationSpeed;
}

void AProjectJoolaGameModeBase::InstantiateWireframeSoundWaveSphere(FVector loc, float radius)
{
	ASoundSphere* SoundSphere = (ASoundSphere*)GetWorld()->SpawnActor(SoundSphereBlueprint, &loc, &FRotator::ZeroRotator);
	SoundSphere->TargetSphereRadius = radius;
	SoundSphere->PropagationSpeed = PropagationSpeed;
}

void AProjectJoolaGameModeBase::PlaceLeversInLevel()
{
	TArray<AActor*> PotentialLeverLocations;

	for (const TPair<FName, int> pair : TagLeversAssociationMap)
	{
		int count = pair.Value;
		TArray<UInteractableSwitch*> InteractableLevers;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), pair.Key, PotentialLeverLocations);

		if (PotentialLeverLocations.Num() < count)
			continue;
		while (count > 0)
		{
			int index = FMath::TruncToInt(FMath::RandRange(0, PotentialLeverLocations.Num() - 1));

			if (!PotentialLeverLocations[index]->GetClass()->IsChildOf(ALeverSpawnPoint::StaticClass()))
				continue;

			AActor* bp = GetWorld()->SpawnActor(LeverBlueprint,
				new FVector(PotentialLeverLocations[index]->GetActorLocation()),
				new FRotator(PotentialLeverLocations[index]->GetActorRotation())
			);

			InteractableLevers.Add(bp->FindComponentByClass<UInteractableSwitch>());

			for (int i = 0; i < PotentialLeverLocations[index]->Tags.Num(); i++)
				bp->Tags.Add(PotentialLeverLocations[index]->Tags[i]);

			PotentialLeverLocations[index]->Destroy();
			PotentialLeverLocations.RemoveAt(index);
			bp = nullptr;
			count--;
		}
		for (int i = 0; i < PotentialLeverLocations.Num(); i++)
		{
			if (PotentialLeverLocations[i]->GetClass()->IsChildOf(ALeverSpawnPoint::StaticClass()))
			{
				PotentialLeverLocations[i]->Destroy();
				continue;
			}

			if (PotentialLeverLocations[i]->FindComponentByClass<UInteractableDoorLocked>())
			{
				PotentialLeverLocations[i]->FindComponentByClass<UInteractableDoorLocked>()->InteractableLeversToCheck = InteractableLevers;
				PotentialLeverLocations[i]->FindComponentByClass<UInteractableDoorLocked>()->RequiredNumberOfActivatedActors = InteractableLevers.Num();
				for (int j = 0; j < InteractableLevers.Num(); j++)
				{
					InteractableLevers[j]->ActorsToInteract.Add(PotentialLeverLocations[i]);
				}
			}
		}
		PotentialLeverLocations.Empty();
	}

}

void AProjectJoolaGameModeBase::PlayAlarmSound(FVector loc)
{
	/*if (!AlarmAudioComponent->IsPlaying())
	{
		AlarmAudioComponent->SetWorldLocation(loc);
		AlarmAudioComponent->Play();
	}*/

	for (int i = 0; i < Speakers.Num(); i++)
	{
		Cast<ASpeaker>(Speakers[i])->PlaySpeakerSound();
	}

	//Emplacement temporaire
	if (!bZone2Unlocked)
	bZone2Unlocked = true;
}

void AProjectJoolaGameModeBase::PlayPursuitMusic(float fadein,float fadeout,float timefadeout)
{
	StableMusicAudioComponent->FadeOut(fadeout, timefadeout);
	PursuitMusicAudioComponent->FadeIn(fadein);
}

void AProjectJoolaGameModeBase::StopMusic(float fadeout, float timefadeout)
{
	PursuitMusicAudioComponent->FadeOut(fadeout, timefadeout);
	StableMusicAudioComponent->FadeOut(fadeout, timefadeout);
}

void AProjectJoolaGameModeBase::PlayStableMusic(float fadein, float fadeout, float timefadeout)
{
	PursuitMusicAudioComponent->FadeOut(fadeout, timefadeout);
	StableMusicAudioComponent->FadeIn(fadein);
}



