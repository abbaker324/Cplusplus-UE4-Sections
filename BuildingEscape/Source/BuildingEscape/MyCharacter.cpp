// Copyright Andrew Baker 2016

#include "BuildingEscape.h"
#include "MyCharacter.h"


// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	this->Jump();
	this->Crouch();
	this->AddMovementInput(FVector(1.0f, 0.f, 0.f), 1.f);
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

