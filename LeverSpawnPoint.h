
// Actor used as a reference point for an interactive actor's location and rotation.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "LeverSpawnPoint.generated.h"

UCLASS()
class PROJECTJOOLA_API ALeverSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALeverSpawnPoint();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* Root;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* LeverBaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* LeverHandleMesh;

	UPROPERTY(EditAnywhere, Category = "Tag")
		FName TagToSearchSP;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
