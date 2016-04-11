// Copyright Andrew Baker 2016

#include "BuildingEscape.h"
#include "Grabber.h"


#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));
	
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Get player view point this tick.
	FVector Location;
	FRotator Rotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT Location, OUT Rotation
	);

	//UE_LOG(LogTemp, Warning, TEXT("Player view point information: [Locaton: %s] [Rotation: %s]"),
	//	*Location.ToString(), *Rotation.ToString()
	//);

	//Raycast out to reach distance.
	FVector LineTraceEnd = Location + (Rotation.Vector() * this->Reach);

	//See what we hit.
	DrawDebugLine(this->GetWorld(), Location, LineTraceEnd, FColor(255, 0, 0), false, 0.f, 0, 10.f);
}

