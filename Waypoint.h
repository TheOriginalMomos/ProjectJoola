

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Waypoint.generated.h"

UCLASS()
class PROJECTJOOLA_API AWaypoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaypoint();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AWaypoint*> NextWaypoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AWaypoint* NextZone2Waypoint = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AWaypoint* NextZone3Waypoint = nullptr;

	AWaypoint* RandomisePatterm();
	int GetIncStepOn() { return IncStepOn; };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int IncStepOn = 1;
	bool b2IsUnlocked = false;
	bool b3IsUnlocked = false;

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnPlayerEnter(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bfromsweeep, const FHitResult &SweepResult);

	void UnlockNextZone(int Zone);

};
