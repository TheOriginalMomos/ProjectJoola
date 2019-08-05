

#pragma once

#include "CoreMinimal.h"
#include "WalkStance.h"
#include "Monster.h"
#include "PJCharacter.h"
#include "GameFramework/Character.h"
#include "Sound/SoundCue.h"
#include "SoundSphere.h"
#include "Engine.h"
#include "PlayerHUD.h"
#include "InteractableDoorLocked.h"
#include "InteractableThrowable.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "MonsterPrisoner.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTJOOLA_API AMonsterPrisoner : public AMonster
{
	GENERATED_BODY()
	
public:
	AMonsterPrisoner();

	UPROPERTY()
		APlayerHUD* PlayerHUD;

	UPROPERTY()
		APlayerController* PlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		USoundCue* ScreamSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* ArmHitbox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class USkeletalMeshComponent* MonsterMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName BoneName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TimeLook = 200;

	void SetCharacterTooclose(bool TooClose);

	UFUNCTION()
		void OnHearing(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bfromsweeep, const FHitResult &SweepResult);

	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* overlapComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);
	

protected:

	void BeginPlay();
	void Tick(float DeltaTime);
	void Scream();
	void PlayerSafety(bool state);
	void OpenDoor();
	void SetPlayerInvincibility(bool state);

	FHitResult Hit;
	
	AActor* DoorToOpen;
	UInteractableBase* InteractableActor;

	APJCharacter* Player;

	FTimerHandle timerScream;
	
	
	bool bStableMusicStarted = false;
	bool bIsPlayerInvincible = false;
	bool bIsPlayerMute = false;
};
