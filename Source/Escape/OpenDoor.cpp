// Fill out your copyright notice in the Description page of Project Settings.

#include "Escape.h"
#include "OpenDoor.h"
#include "Grabber.h"

#define OUT
// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();

}

void UOpenDoor::OpenDoor()
{
	Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
}
void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the Trigger Volume
	if (GetTotalMass() >= 100.f) {

		//if total mass > threshold

		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();

	}

	//Check if time to close door
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay) {

		CloseDoor();
	}

}

float UOpenDoor::GetTotalMass()
{
	float TotalMass = 0.0;
	//Find overlapping actors
	TArray<AActor*> OverlappingActors;

	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

		//Iterate through adding masses

	for (const auto* Actors: OverlappingActors) 
	{
		

		TotalMass += Actors->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *Actors->GetName());
		UE_LOG(LogTemp, Warning, TEXT("%f on pressure plate"), TotalMass);

	}


	return TotalMass;
}


