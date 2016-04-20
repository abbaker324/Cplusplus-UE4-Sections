// Copyright Andrew Baker 2016

#include "BuildingEscape.h"
#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	if(PressurePlate)
	{
		PressurePlate->OnActorBeginOverlap.AddDynamic(this, &UOpenDoor::ProcessPressurePlate);
		PressurePlate->OnActorEndOverlap.AddDynamic(this, &UOpenDoor::ProcessPressurePlate);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s does not have a Pressure Plate."), *GetOwner()->GetName());
	}
}

void UOpenDoor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (PressurePlate)
	{
		PressurePlate->OnActorBeginOverlap.RemoveAll(this);
		PressurePlate->OnActorEndOverlap.RemoveAll(this);
	}

	Super::EndPlay(EndPlayReason);
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

//Callback function for PressurePlate->OnActorBeginOverlap
//Total up the mass of all actors on pressure plate. If it exceeds trigger mass, broadcast OnOpen, otherwise broadcast OnClose
void UOpenDoor::ProcessPressurePlate(AActor* actor)
{
	UE_LOG(LogTemp, Error, TEXT("%s->ProcessPressurePlate!"), *GetOwner()->GetName());

	float TotalMass = 0.f;

	if (PressurePlate)
	{
		TArray<AActor*> OverlappingActors;
		PressurePlate->GetOverlappingActors(OUT OverlappingActors);

		for (const auto& Actor : OverlappingActors)
		{
			UPrimitiveComponent* Mesh = Actor->FindComponentByClass<UPrimitiveComponent>();

			if (Mesh)
			{
				TotalMass += Mesh->GetMass();
			}
		}

		if (TotalMass > TriggerMass) //TODO: Replace hard coded value with a parameter.
		{
			OnOpen.Broadcast();
		}
		else
		{
			OnClose.Broadcast();
		}
	}
}

void UOpenDoor::OpenDoor()
{
	OnOpen.Broadcast();
}

void UOpenDoor::CloseDoor()
{
	OnClose.Broadcast();
}
