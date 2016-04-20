// Copyright Andrew Baker 2016

#pragma once

#include "GameFramework/Actor.h"
#include "Countdown.generated.h"

UCLASS()
class BUILDINGESCAPE_API ACountdown : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACountdown();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//How long before the count down expires.
	UPROPERTY(EditAnywhere, Category="Bullshit")
	uint32 CountdownTime = 3;

	UTextRenderComponent* CountdownText;

	void UpdateTimerDisplay();
	void AdvanceTimer();

	UFUNCTION(BlueprintNativeEvent)
	void CountdownHasFinished();
	void CountdownHasFinished_Implementation();

	FTimerHandle CountdownTimerHandle;
	
};
