

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "PJCharacter.h"
#include "PlayerHUD.h"
#include "EndGameGoal.generated.h"

UCLASS()
class PROJECTJOOLA_API AEndGameGoal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEndGameGoal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
		APlayerHUD* PlayerHUD;
	UPROPERTY()
		APlayerController* PlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		UBoxComponent* EndZoneTrigger;

	//Sound Related Variables

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundCue* EndMusic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		UAudioComponent* EndAudioComponent;

	APJCharacter* OnlyHim;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** called when something enters the sphere component */
	UFUNCTION()
		void OnOverlapBeginEndZoneTrigger(UPrimitiveComponent* OverlappedComponent , AActor* OtherActor,  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
};
