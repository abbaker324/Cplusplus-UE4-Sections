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

// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	APawn* playerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	float TotalMass = GetTotalMassOfActorsOnPlate();

	//Poll the trigger volume.
	if (TotalMass > TriggerMass) //TODO: Replace hard coded value with a parameter.
	{
		OnOpen.Broadcast();
	} 
	else
	{
		OnClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() 
{
	float totalMass = 0.f;

	if (PressurePlate)
	{
		TArray<AActor*> OverlappingActors;
		PressurePlate->GetOverlappingActors(OUT OverlappingActors);

		for (const auto& Actor : OverlappingActors)
		{
			UPrimitiveComponent* Mesh = Actor->FindComponentByClass<UPrimitiveComponent>();

			if (Mesh)
			{
				totalMass += Mesh->GetMass();
			}
		}
	}

	return totalMass;
}