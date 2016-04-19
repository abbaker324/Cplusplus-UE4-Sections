// Copyright Andrew Baker 2016

#include "BuildingEscape.h"
#include "../Public/MyPawn.h"


// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	//Create a dummy root component we can attach things to.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	//Create a camera and a visible object.
	UCameraComponent* OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
	OurVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));
	//Attach the camera and visible object to our root component. Offset and rotate the camera.
	OurCamera->AttachTo(RootComponent);
	OurCamera->SetRelativeLocation(FVector(-250.f, 0.f, 250.f));
	OurCamera->SetRelativeRotation(FRotator(-45.f, 0.0f, 0.0f));

	OurVisibleComponent->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	float CurrentScale = OurVisibleComponent->GetComponentScale().X;

	if (bIsGrowing)
	{
		CurrentScale += DeltaTime;
	}
	else
	{
		CurrentScale -= DeltaTime * 0.5f;
	}
	CurrentScale = FMath::Clamp(CurrentScale, 0.f, 1.f);
	OurVisibleComponent->SetWorldScale3D(FVector(CurrentScale));

	if (!CurrentVelocity.IsZero())
	{
		FVector newLocation = GetActorLocation() + (CurrentVelocity * DeltaTime);
		SetActorLocation(newLocation);
	}


}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAction("Grow", IE_Pressed, this, &AMyPawn::StartGrowing);
	InputComponent->BindAction("Grow", IE_Released, this, &AMyPawn::StopGrowing);

	InputComponent->BindAxis("MoveX", this, &AMyPawn::Move_XAxis);
	InputComponent->BindAxis("MoveY", this, &AMyPawn::Move_YAxis);
}

void AMyPawn::Move_XAxis(float AxisValue)
{
	CurrentVelocity.X = AxisValue * 100.f;
}

void AMyPawn::Move_YAxis(float AxisValue)
{
	CurrentVelocity.Y = AxisValue * 100.f;
}

void AMyPawn::StartGrowing()
{
	bIsGrowing = true;

}

void AMyPawn::StopGrowing()
{
	bIsGrowing = false;
}
