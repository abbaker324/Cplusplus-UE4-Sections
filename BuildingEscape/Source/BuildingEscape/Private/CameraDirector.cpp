// Copyright Andrew Baker 2016

#include "BuildingEscape.h"
#include "CameraDirector.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ACameraDirector::ACameraDirector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACameraDirector::BeginPlay()
{
	Super::BeginPlay();

	//Set the target view of the player controller to the first camera in the array, if one exists.
	if (ManagedCameras.Num() > 0)
	{
		ProcessTransition();
	}
}

// Called every frame
void ACameraDirector::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	
	if (ManagedCameras.Num() > 0)
	{
		TimeToNextCameraChange -= DeltaTime;
		if (TimeToNextCameraChange <= 0.f)
		{
			ProcessTransition();
		}
	}
}

void ACameraDirector::ProcessTransition()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (PlayerController)
	{
		FCameraStruct CameraData = ManagedCameras[CurrentCameraIndex];
		AActor* Camera = CameraData.Camera;
		float LinearBlend = CameraData.LinearBlend;

		if (Camera)
		{
			if (LinearBlend > 0.f)
			{
				PlayerController->SetViewTargetWithBlend(Camera, LinearBlend);
			}
			else
			{
				PlayerController->SetViewTarget(Camera);
			}
		}
	}

	CurrentCameraIndex++;
	CurrentCameraIndex = CurrentCameraIndex == ManagedCameras.Num() ? 0 : CurrentCameraIndex;
	TimeToNextCameraChange = ManagedCameras[CurrentCameraIndex].TransitionTime;
}