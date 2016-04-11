// Copyright Andrew Baker 2016

#include "BuildingEscape.h"
#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

}

void UOpenDoor::OpenDoor() {
	AActor* owner = this->GetOwner();

	FRotator rotation = FRotator(0.0f, openAngle, 0.0f);

	owner->SetActorRotation(rotation);
}

void UOpenDoor::CloseDoor() {
	AActor* owner = this->GetOwner();

	FRotator rotation = FRotator(0.0f, 0.0f, 0.0f);

	owner->SetActorRotation(rotation);
}

// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	APawn* playerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	//Poll the trigger volume.
	if (this->PressurePlate->IsOverlappingActor(playerPawn)) 
	{
		OpenDoor();
		this->LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	
	//Check if its time to close the door.
	float currentTime = GetWorld()->GetTimeSeconds();

	if (currentTime - this->LastDoorOpenTime >= this->DoorCloseDelay) {
		this->CloseDoor();
	}
}

