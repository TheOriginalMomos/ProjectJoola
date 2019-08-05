// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/World.h"
#include "SoundSphere.h"
#include "CoreMinimal.h"
#include "LeverSpawnPoint.h"
#include "Sound/SoundCue.h"
#include "Waypoint.h"
#include "Speaker.h"
#include "Map.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectJoolaGameModeBase.generated.h"

/**
 *
 */
UCLASS()
class PROJECTJOOLA_API AProjectJoolaGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	AProjectJoolaGameModeBase();

	virtual void BeginPlay() override;

	void InstantiateSoundWaveSphere(FVector loc, float radius);
	void InstantiateWireframeSoundWaveSphere(FVector loc, float radius);
	void PlaceLeversInLevel();
	void PlayStableMusic(float fadein, float fadeout, float timefadeout);
	void PlayPursuitMusic(float fadein, float fadeout, float timefadeout);
	void StopMusic(float fadeout, float timefadeout);
	bool GetIsZone2Unlocked() { return bZone2Unlocked; };
	void IncdoorOpened() { doorOpened++; };
	int getdoorOpened() { return doorOpened; };

	void PlayAlarmSound(FVector loc); // Play the alarm sound when a door wired to levers opens. Doesn't play again if sound is already playing

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundCue* AlarmSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		USoundCue* StableMusic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		USoundCue* PursuitMusic;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		UAudioComponent* AlarmAudioComponent;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		TArray<AActor*> Speakers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UClass* LeverBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UClass* SoundSphereBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		float PropagationSpeed = 750.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lever")
		TMap<FName, int> TagLeversAssociationMap;

protected:
	
	UAudioComponent* StableMusicAudioComponent;
	UAudioComponent* PursuitMusicAudioComponent;

	int doorOpened = 0;
	bool bZone2Unlocked = false;
};