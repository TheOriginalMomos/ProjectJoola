

#include "MonsterPrisoner.h"

AMonsterPrisoner::AMonsterPrisoner()
{
	GetCapsuleComponent()->InitCapsuleSize(60.f, 108.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	ScreamAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ScreamSound"));
	ScreamAudioComponent->bAutoActivate = false;
	ScreamAudioComponent->SetupAttachment(GetCapsuleComponent());

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Ears"));
	BoxComponent->SetupAttachment(GetRootComponent());
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AMonsterPrisoner::OnHearing);

	ArmHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("ArmHitbox"));
	ArmHitbox->SetupAttachment(GetRootComponent());

	CharacterMovement->bOrientRotationToMovement = true;
	CharacterMovement->RotationRate = FRotator(0.0f, 1000.f, 0.0f);

	CharacterMovement->MaxWalkSpeed = GetCharWalkSpeed();
}

void AMonsterPrisoner::Tick(float DeltaTime)
{
	if (ArmHitbox && MonsterMesh)
	ArmHitbox->SetWorldLocationAndRotation(MonsterMesh->GetBoneLocation(BoneName), MonsterMesh->GetBoneQuaternion(BoneName));

	CurrentStance->UpdateStance(*this);

	if (!Player->GetPlayerInvincibility())
		SetPlayerInvincibility(false);
	else
		SetPlayerInvincibility(true);

	if (!Player->GetPlayerSoundless())
		bIsPlayerMute = false;
	else
		bIsPlayerMute = true;

	OpenDoor();
}

void AMonsterPrisoner::BeginPlay()
{
	Super::BeginPlay();
	
	CharacterTooClose = false;
	TArray<USkeletalMeshComponent*> SkeletalMeshes;
	this->GetComponents<USkeletalMeshComponent>(SkeletalMeshes);

	Player = Cast<APJCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD());

	MonsterMesh = SkeletalMeshes[0];

	ArmHitbox->OnComponentBeginOverlap.AddDynamic(this, &AMonsterPrisoner::OnComponentBeginOverlap);
	
	if (ScreamAudioComponent->IsValidLowLevelFast()) {
		ScreamAudioComponent->SetSound(ScreamSound);
	}

	PlayerSafety(true);
	CurrentStance = new WalkStance(*this);


}

void AMonsterPrisoner::Scream()
{
	ScreamAudioComponent->Play();
}

void AMonsterPrisoner::SetCharacterTooclose(bool TooClose)
{
	if (CurrentStance->GetStanceName() == FString(TEXT("Pursuit")))
	{
		CharacterTooClose = TooClose;
	}
		
}

void AMonsterPrisoner::OnHearing(UPrimitiveComponent* OverlapComp, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bfromsweeep, const FHitResult &SweepResult)
{
	
	ASoundSphere* Sound = nullptr;

	if (OtherActor != nullptr && !bIsPlayerMute)
	{
		Sound = Cast<ASoundSphere>(OtherActor);
		if (Sound != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("I hear someone at %f %f %f"),Sound->GetSpherePos().X,Sound->GetSpherePos().Y,Sound->GetSpherePos().Z ));
			PosLastHeard = Sound->GetSpherePos();
			PosLastHeard.Z += 50;
			SetIsHearing(true);
		}
	}
}

void AMonsterPrisoner::OnComponentBeginOverlap(UPrimitiveComponent * overlapComponent, AActor * otherActor, UPrimitiveComponent * otherComponent, int32 otherBodyIndex, bool bFromSweep, const FHitResult & sweepResult)
{
	if (GEngine && otherActor->GetName() == "BP_PJCharacter_C_0")
	{
		//GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Green, overlapComponent->GetName() + " collided with " + otherActor->GetName());
		Player->PlaySoundDying();
		//Ici <-----------------------------------------------------------------------------------------------------------------------------------
		PlayerHUD->ShowDeathScreenMenu();
	}
		
}

void AMonsterPrisoner::PlayerSafety(bool state)
{
	if (state || bIsPlayerInvincible)
		ArmHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	else
		ArmHitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMonsterPrisoner::OpenDoor()
{
	FVector StartRay = GetRootComponent()->GetComponentLocation();
	FVector FowardRay = GetRootComponent()->GetForwardVector();
	FVector EndRay = ((FowardRay * 300.f) + StartRay);
	FCollisionQueryParams RayParam;

	GetWorld()->LineTraceSingleByChannel(Hit, StartRay, EndRay, ECC_Visibility, RayParam);
	DrawDebugLine(GetWorld(), StartRay, EndRay, FColor(255, 205, 153),false);

	if (Hit.bBlockingHit && DoorToOpen != Hit.GetActor())
	{
		DoorToOpen = static_cast<AActor*>(Hit.GetActor());
		InteractableActor = DoorToOpen->FindComponentByClass<UInteractableDoor>();

		if (InteractableActor)
		{
			if (CurrentStance->GetStanceName() == FString(TEXT("Pursuit")) || CurrentStance->GetStanceName() == FString(TEXT("Search")))
			{
				if (!DoorToOpen->FindComponentByClass<UInteractableDoorLocked>())
				{
					Cast<UInteractableDoor>(InteractableActor)->SetMonsterUsage(true);
					Cast<UInteractableDoor>(InteractableActor)->SetDirection(this);
					Cast<UInteractableDoor>(InteractableActor)->BreakDoorOpen();
				}
			}
			else
			{
				if (!DoorToOpen->FindComponentByClass<UInteractableDoorLocked>())
				{
					Cast<UInteractableDoor>(InteractableActor)->SetMonsterUsage(true);
					Cast<UInteractableDoor>(InteractableActor)->SetDirection(this);
					Cast<UInteractableDoor>(InteractableActor)->Interact();
				}
			}
			
		}
		
	}
	
}

void AMonsterPrisoner::SetPlayerInvincibility(bool state)
{
	bIsPlayerInvincible = state;
	
	if (CurrentStance->GetStanceName() == FString(TEXT("Pursuit")))
	{
		PlayerSafety(false);
	}
	else
	{
		PlayerSafety(true);
	}
}



