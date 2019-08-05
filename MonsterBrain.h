#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PJCharacter.h"
#include "EngineUtils.h"
#include "TimerManager.h"
#include "Monster.h"
#include "MonsterBrain.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTJOOLA_API AMonsterBrain : public AAIController
{
	GENERATED_BODY()


public:
	
	AMonsterBrain();
	virtual void BeginPlay() override;
	virtual void Possess(APawn* Pawn) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual FRotator GetControlRotation() const override;


	UFUNCTION()
		virtual void OnPawnDetected(const TArray<AActor*> &DetectedPawns);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AISightRadius = 3000;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AISightAge = 5.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AILoseSightRadius = AISightRadius + 50.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AISightAngle = 75.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		bool Isvisible = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float DistanceTarget = 200.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		int FrameScream = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stamina)
		float DebugDecayTickRate = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CharacterTooClose = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool PlayerInSight = false;

protected:

	void CheckifVisible();
	virtual void MonsterMechanics();
	bool CheckForLight();
	template<typename T>
	void FindAllActors(UWorld* World, TArray<T*>& Out);
	void GetLightSource();
	void GetMonsterPosition();
	void ResetMonster();

	FVector CurrentPosition = FVector(0, 0, 0);
	FTimerHandle DebugTickTimerHandle;
	APJCharacter* Player = nullptr;
	FVector PlayerLastSeen = FVector(0, 0, 0);
	AMonster* Monster;
	AWaypoint* LastWaypoint;
	bool PlayerInCircle = false;//La vision ne sait pas si le joueur entre ou sort de la distance
};
