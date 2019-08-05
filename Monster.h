
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProjectJoolaGameModeBase.h"
#include "Waypoint.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Monster.generated.h"

UCLASS()
class PROJECTJOOLA_API AMonster : public ACharacter
{
	GENERATED_BODY()
	friend class MonsterStance;
	friend class PrSearchStance;
	friend class WalkStance;
	friend class PrPursuitStance;
	friend class PrLookAroundStance;
	friend class PrLookSound;

public:
	// Sets default values for this character's properties
	AMonster(); 
	MonsterStance* CurrentStance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UAudioComponent* ScreamAudioComponent;

	AProjectJoolaGameModeBase* GameMode;

	void ChangeStance();
	bool InSight = false;
	bool InMind = false;
	bool IsLooking = false;
	bool IsHearing = false;
	bool bSongStarted = false;
	FVector PosLastHeard;
	
	AWaypoint* LastWaypoint;

	virtual void Scream();
	virtual void PlayerSafety(bool state); ///Bool : Precaution pour le joueur Ex(ne peut pas etre tue si XXX donc nous enlevons la hitbox)

public:	

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AWaypoint* NextWaypoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CharacterTooClose = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CharacterRunSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CharacterWalkSpeed = 301.f;

	virtual void Tick(float DeltaTime) override;
	AWaypoint* GetLastWaypoint() { return LastWaypoint; };

	void SetInSight(bool IsInSight);
	void SetInMind(bool IsInMind);
	void SetIsLookingTrue();
	virtual void SetCharacterTooclose(bool TooClose) { CharacterTooClose = TooClose; };
	void SetIsHearing(bool IsInHearing);
	void SetLastWaypoint(AWaypoint* Last) { LastWaypoint = Last; };
	void SetSongStarted(bool bsong) { bSongStarted = bsong; };

	bool GetCharacterTooClose() { return CharacterTooClose; };
	bool GetSongStarted() { return bSongStarted; };
	bool GetInMind() { return InMind; };
	bool GetInSight() { return InSight; };
	bool GetIsLooking() { return IsLooking; };
	bool GetIsHearing() { return IsHearing; };
	float GetCharWalkSpeed() { return CharacterWalkSpeed; }
	float GetCharRunSpeed() { return CharacterRunSpeed; }

	AProjectJoolaGameModeBase* GetGameMode() { return GameMode; };
	UCharacterMovementComponent* CharacterMovement = GetCharacterMovement();
	FVector GetPosLastHeard() { return PosLastHeard; };
	
};
