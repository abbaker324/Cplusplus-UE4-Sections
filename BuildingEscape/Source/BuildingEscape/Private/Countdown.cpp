// Copyright Andrew Baker 2016

#include "BuildingEscape.h"
#include "Countdown.h"


// Sets default values
ACountdown::ACountdown()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CountdownText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent"));
	CountdownText->SetHorizontalAlignment(EHTA_Center);
	CountdownText->SetWorldSize(150.f);
	RootComponent = CountdownText;
}

// Called when the game starts or when spawned
void ACountdown::BeginPlay()
{
	Super::BeginPlay();
	
	UpdateTimerDisplay();

	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &ACountdown::AdvanceTimer, 1.0f, true);
}

// Called every frame
void ACountdown::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ACountdown::UpdateTimerDisplay()
{
	CountdownText->SetText(FString::FromInt(FMath::Max<uint32>(CountdownTime, 0)));
}

void ACountdown::AdvanceTimer()
{
	--CountdownTime;

	UpdateTimerDisplay();

	if (CountdownTime < 1)
	{
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
		CountdownHasFinished();
	}
}

void ACountdown::CountdownHasFinished_Implementation()
{
	CountdownText->SetText("GO!");
}
