// Character actor. Can run, crouch, walk, pick up objects, etc.

#pragma once

#include "OutputDeviceNull.h"
#include "Camera/CameraShake.h"
#include "Engine/EngineTypes.h"
#include "Engine/Engine.h"
#include "Components/TimelineComponent.h"
#include "ProjectJoolaGameModeBase.h"
#include "Sound/SoundCue.h"
#include "Components/SpotLightComponent.h"
#include "LightVision.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "PJCharacter.generated.h"

UCLASS(config = Game)
class PROJECTJOOLA_API APJCharacter : public ACharacter
{
	GENERATED_BODY()

		/** First person camera */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FirstPersonCameraComponent;

	UCharacterMovementComponent* CharacterMovement = GetCharacterMovement();

public:
	// Sets default values for this character's properties
	APJCharacter();

	AProjectJoolaGameModeBase* GameMode;
	APlayerCameraManager* PlayerCamManager;

	/** Holding Component */
	UPROPERTY(EditAnywhere)
		class USceneComponent* HoldingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		USoundCue* SEDying;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		USoundCue* SECrouching;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		USoundCue* SEWalking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		USoundCue* SERunning;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		USoundCue* SEFlashlightOn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		USoundCue* SEBreathing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		int WalkSpeed = 450;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		int CrouchSpeed = 250;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		int RunSpeed = 900;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		int RunSoundwaveRadius = 1500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		int WalkSoundwaveRadius = 750;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		int CrouchSoundwaveRadius = 250;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float WalkSoundDelay = .75f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float RunSoundDelay = .5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float CrouchSoundDelay = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Flashlight)
		USpotLightComponent* FlashlightComponent;

	UPROPERTY(EditDefaultsOnly, Category = Flashlight)
		TSubclassOf<class ALightVision> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		UTimelineComponent* TimelineComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		UCurveFloat* CurveFloat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		TSubclassOf<UCameraShake> CameraShakeWalkingForward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		TSubclassOf<UCameraShake> CameraShakeRunning;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		TSubclassOf<UCameraShake> CameraShakeCrouching;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float StartupDelay = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stamina)
		float StaminaDecayTickRate = .2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stamina)
		float StaminaRegenTickRate = .2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stamina)
		float StaminaRegenDelay = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stamina)
		int StaminaValue = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stamina)
		int StaminaMaxValue = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stamina)
		int StaminaRegenValue = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stamina)
		int StaminaDecayValue = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vision)
		int IdleCrouchVision = 200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vision)
		int MovCrouchVision = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vision)
		int IdleVision = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vision)
		int MovVisionWalk = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vision)
		int VisionRun = 1500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vision)
		int FLAddVision = 500; ///Flashlight

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vision)
		int MaxVision = 3000; 

	int GetVisibility() { return Visibility; };

	void PlaySoundDying();
	bool GetPlayerInvisibility() { return bIsInvisible; };
	bool GetPlayerInvincibility() { return bIsInvincible; };
	bool GetPlayerSoundless() { return bIsMute; };

private:

	void StartupDelayCompleted();
	void MoveForward(float Val);
	void MoveRight(float Val);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void StartRunning();
	void ToggleRunning();
	void EndRunning();
	void ToggleCrouching();
	void ToggleFlashLight();
	void GenerateMovementSounds();
	void StaminaDecayTick();
	void StaminaRegenDelayTick();
	void StaminaRegenTick();
	void PlaySoundRunning();
	void PlaySoundWalking();
	void PlaySoundCrouching();
	void PlaySoundFlashlightOn();
	void PlaySoundBreathing();
	void CalculateVisibility();
	bool CheckForCeiling();
	bool IsMoving();
	void GenerateLight();

	UFUNCTION()
		void TimelineCallback(float val);
	UFUNCTION()
		void TimelineFinishedCallback();

	UPROPERTY()
		TEnumAsByte<ETimelineDirection::Type> TimelineDirection;

	bool bIsRunning = false;
	bool bIsCrouched = false;
	bool bCanRun = true;
	bool bIsMovingAlongX = false;
	bool bIsMovingAlongY = false;
	bool bIsFlashlightOn = true;
	
	//Cheats-------------------------------
	bool bIsInvincible = false; 
	bool bIsInvisible = false;
	bool bIsMute = false;

	void PlayerInvisible();
	void PlayerInvincible();
	void PlayerMute();
	//-------------------------------------

	int cptLight = 0;
	int Visibility = 0;
	float CapsuleHalfHeightStanding;

	FVector PositionNormalCamera = FVector(0.f, 1.75f, 64.f);

	FTimerHandle StartupDelayTimerHandle;
	FTimerHandle StaminaTickTimerHandle;
	FTimerHandle StaminaRegenDelayTimerHandle;
	FTimerHandle WalkingSoundTimerHandle;
	FTimerHandle RunningSoundTimerHandle;
	FTimerHandle CrouchedSoundTimerHandle;
	FTimerHandle* CurrentSoundTimer;

	UAudioComponent* AudioComponentCrouching;
	UAudioComponent* AudioComponentWalking;
	UAudioComponent* AudioComponentRunning;
	UAudioComponent* AudioComponentFlashlightOn;
	UAudioComponent* AudioComponentBreathing;
	UAudioComponent* AudioComponentDying;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};
