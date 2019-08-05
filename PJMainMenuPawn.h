
// Player pawn used for the main menu scene. Can't move. Only used for camera transitions and such.


#pragma once

#include "Engine/EngineTypes.h"
#include "Engine/Engine.h"
#include "GameFramework/Actor.h"
#include "ProjectJoolaGameModeMainMenu.h"
#include "CameraLocationsContainer.h"
#include "Components/InputComponent.h"
#include "Components/TimelineComponent.h"
#include "PlayerHUD.h"

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PJMainMenuPawn.generated.h"

UCLASS()
class PROJECTJOOLA_API APJMainMenuPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APJMainMenuPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		UCurveFloat* CurveFloat;

	UPROPERTY()
		TEnumAsByte<ETimelineDirection::Type> TimelineDirection;

	UPROPERTY()
		UTimelineComponent* TimelineComponent;

	UPROPERTY()
		AProjectJoolaGameModeMainMenu* GameMode;

	UPROPERTY()
		APlayerHUD* PlayerHUD;

	UPROPERTY()
		ACameraLocation* CurCam;

	UPROPERTY()
		ACameraLocation* NewCam;

	ACameraLocationsContainer* ActorContainer;

	FTimerHandle StartupDelayTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float StartupDelay = 10.f;

	bool TransitioningToGame = false;
	bool TransitioningToMenu = false;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveCameraInstantly(const FVector newLoc, const FRotator newRot);
	void ExecuteIntroTransition();
	void ExecuteGameTransition();
	void ChangeCameraIndex(float val);

	UFUNCTION()
		void TimelineCallback(float val);
	UFUNCTION()
		void TimelineFinishedCallback();

	UFUNCTION(BlueprintCallable)
		void BtnPlayGame_Clicked();

	UFUNCTION(BlueprintCallable)
		void BtnSettings_Clicked();

	UFUNCTION(BlueprintCallable)
		void BtnHowTo_Clicked();

};
