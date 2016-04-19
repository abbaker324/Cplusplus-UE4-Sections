// Copyright Andrew Baker 2016

#pragma once

#include "GameFramework/Actor.h"
#include "CameraDirector.generated.h"

USTRUCT(BlueprintType)
struct FCameraStruct
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TransitionTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LinearBlend;

	FCameraStruct()
	{
		Camera = nullptr;
		TransitionTime = 3.f;
		LinearBlend = 0.f;
	}

};

UCLASS()
class BUILDINGESCAPE_API ACameraDirector : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACameraDirector();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void ProcessTransition();


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCameraStruct> ManagedCameras;
	
	FCameraStruct* CurrentCamera = nullptr;
	int CurrentCameraIndex = 0;

	float TimeToNextCameraChange = 0.f;
	
};
