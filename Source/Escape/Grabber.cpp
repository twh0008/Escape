// Taylor Hartley Copyright 2017

#include "Escape.h"
#include "Grabber.h"

#define OUT
// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandleComponent();
	SetupInputComponent();
	

}

void UGrabber::SetupInputComponent() {
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		UE_LOG(LogTemp, Warning, TEXT("%s contains Input"), *GetOwner()->GetName());
		//bind input axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("%s is missing Input"), *GetOwner()->GetName());
	}
}



void UGrabber::FindPhysicsHandleComponent() {
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle) {

		//physics handle found
	}
	else {

		UE_LOG(LogTemp, Warning, TEXT("%s is missing Handler"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grab is pressed"));

	///LINE TRACE and Try and reach any actors with physics body collision channel
	
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	if (ActorHit) {
		PhysicsHandle->GrabComponent(ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true);
	}
	///If we hit something, attach physics handle
	//TODO attach physics

}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Grab is released"));

	PhysicsHandle->ReleaseComponent();

	//TODO release physics

}



// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	// if physics handle is attached, move object that is held

	if (PhysicsHandle->GrabbedComponent) {

		PhysicsHandle->SetTargetLocation(LineTraceEnd);

	}

}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	// Get Player view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	//Setup Query
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	//Ray-cast out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hitting Object: %s"), *ActorHit->GetName());
	}

	return Hit;
}

