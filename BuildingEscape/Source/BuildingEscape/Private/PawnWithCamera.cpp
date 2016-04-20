// Copyright Andrew Baker 2016

#include "BuildingEscape.h"
#include "PawnWithCamera.h"


// Sets default values
APawnWithCamera::APawnWithCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->AttachTo(RootComponent);

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArm->SetRelativeLocationAndRotation(FVector(0.f, 0.f, 50.f), FRotator(-60.f, 0.f, 0.f));
	CameraSpringArm->TargetArmLength = 400.f;
	CameraSpringArm->bEnableCameraLag = true;
	CameraSpringArm->CameraLagSpeed = 3.f;
	CameraSpringArm->AttachTo(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->AttachTo(CameraSpringArm, USpringArmComponent::SocketName);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	Mesh->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void APawnWithCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APawnWithCamera::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (bZoomingIn)
	{
		ZoomFactor += DeltaTime * 0.5f;
	}
	else
	{
		ZoomFactor -= DeltaTime * 0.25f;
	}
	ZoomFactor = FMath::Clamp<float>(ZoomFactor, 0.f, 1.f);

	Camera->FieldOfView = FMath::Lerp<float>(90.f, 60.f, ZoomFactor);
	CameraSpringArm->TargetArmLength = FMath::Lerp<float>(400.f, 300.f, ZoomFactor);

	//We rotate the root component when we yaw. This will spin the camera around.
	{
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw += CameraInput.X;
		SetActorRotation(NewRotation);
	}

	//We do not pitch the camera. Instead we pitch the spring arm to bring the camera closer or further from the ground.
	{
		FRotator NewRotation = CameraSpringArm->GetComponentRotation();
		NewRotation.Pitch = FMath::Clamp<float>(NewRotation.Pitch + CameraInput.Y, -80.f, -15.f);
		CameraSpringArm->SetWorldRotation(NewRotation);
	}

	//Now we move the pawn.
	{
		if (!MovementInput.IsZero())
		{
			MovementInput = MovementInput.SafeNormal() * Speed;

			FVector Location = GetActorLocation();
			Location += GetActorForwardVector() * MovementInput.X * DeltaTime;
			Location += GetActorRightVector() * MovementInput.Y * DeltaTime;
			SetActorLocation(Location);
		}
	}
}

// Called to bind functionality to input
void APawnWithCamera::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAxis("MoveX", this, &APawnWithCamera::MoveForward);
	InputComponent->BindAxis("MoveY", this, &APawnWithCamera::MoveRight);
	InputComponent->BindAxis("CameraPitch", this, &APawnWithCamera::PitchCamera);
	InputComponent->BindAxis("CameraYaw", this, &APawnWithCamera::YawCamera);

	InputComponent->BindAction("MouseZoom", IE_Pressed, this, &APawnWithCamera::ZoomIn);
	InputComponent->BindAction("MouseZoom", IE_Released, this, &APawnWithCamera::ZoomOut);


}

void APawnWithCamera::MoveForward(float AxisValue)
{
	MovementInput.X = FMath::Clamp<float>(AxisValue, -1.f, 1.f);
}

void APawnWithCamera::MoveRight(float AxisValue)
{
	MovementInput.Y = FMath::Clamp<float>(AxisValue, -1.f, 1.f);
}

void APawnWithCamera::PitchCamera(float AxisValue)
{
	CameraInput.Y = AxisValue;
}

void APawnWithCamera::YawCamera(float AxisValue)
{
	CameraInput.X = AxisValue;
}

void APawnWithCamera::ZoomIn()
{
	bZoomingIn = true;
}

void APawnWithCamera::ZoomOut()
{
	bZoomingIn = false;
}

