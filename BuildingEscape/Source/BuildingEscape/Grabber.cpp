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

	GetPhysicsHandle();
	GetInput();
	
}

///OnTick: If there is a grabbed component, move it around using the LineTraceEnd vector.
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle->GrabbedComponent)
	{
		UpdateTransformData();
		PhysicsHandle->SetTargetLocation(GetLineTraceEnd());
	}
}


///Called by both the tick method and the raytrace method on demand to update Location and Rotation.
void UGrabber::UpdateTransformData()
{
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PawnLocation, OUT PawnRotator
	);
}

///Gets the first object hit by raytrace from pawn, along direction vector within Reach. If body found, attach it is PhysicsHandle.
void UGrabber::Grab()
{
	auto Hit = this->GetFirstPhysicsObjectFromTrace();
	auto ActorHit = Hit.GetActor();
	
	if (ActorHit) 
	{
		auto ComponentToGrab = Hit.GetComponent();
		PhysicsHandle->GrabComponent(ComponentToGrab, NAME_None, ActorHit->GetActorLocation(), true);

		UE_LOG(LogTemp, Warning, TEXT("Grabbing %s!"), *(ActorHit->GetName()));
	}
}

void UGrabber::Release() 
{
	PhysicsHandle->ReleaseComponent();
}

void UGrabber::GetPhysicsHandle()
{
	this->PhysicsHandle = this->GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing physics handler component."), *(this->GetOwner()->GetName()));
	}
}

void UGrabber::GetInput()
{
	this->Input = this->GetOwner()->FindComponentByClass<UInputComponent>();
	if (this->Input)
	{
		Input->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		Input->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing input component."), *(this->GetOwner()->GetName()));
	}
}

const FVector UGrabber::GetLineTraceEnd() 
{
	UpdateTransformData();
	return PawnLocation + (PawnRotator.Vector() * Reach);
}

const FHitResult UGrabber::GetFirstPhysicsObjectFromTrace()
{
	FHitResult HitResult;
	FCollisionQueryParams queryParams(FName(""), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult, PawnLocation, GetLineTraceEnd(), FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), queryParams
	);

	return HitResult;
}

