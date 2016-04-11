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

const FHitResult UGrabber::GetFirstPhysicsObjectFromTrace()
{
	//Collect spatial information for pawn.
	FVector PawnLocation;
	FRotator PawnRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PawnLocation, OUT PawnRotation
	);

	//Ray trace in direction player is facing.
	FVector LineTraceEnd = PawnLocation + (PawnRotation.Vector() * Reach);

	FHitResult Hit;
	FCollisionQueryParams queryParams(FName(""), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit, PawnLocation, LineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), queryParams
	);

	return Hit;
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	//Collect spatial information for pawn.
	FVector PawnLocation;
	FRotator PawnRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PawnLocation, OUT PawnRotation
	);

	//Ray trace in direction player is facing.
	FVector LineTraceEnd = PawnLocation + (PawnRotation.Vector() * Reach);

	if (PhysicsHandle->GrabbedComponent) 
	{
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

