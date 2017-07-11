// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include <Gameframework/Actor.h>


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
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the trigger volume
	if (GetTotalMassOfActorsOnPlate() >= 40.f)
	{
		// if the ActorThatOpens is in the volume
		OpenDoor();
		LastDoorOpen = GetWorld()->GetTimeSeconds();
	}
	
	// check if it's time to close the door
	if (GetWorld()->GetTimeSeconds() - LastDoorOpen > DoorCloseDelay)
	{
		CloseDoor();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	TArray<AActor*> OverlappingActors;

	if (PressurePlate) return 0;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (auto* actor : OverlappingActors)
	{
		TotalMass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();

		UE_LOG(LogTemp, Warning, TEXT("Placed %s on pressure plate"), *actor->GetName());
	}

	return TotalMass;
}

void UOpenDoor::OpenDoor()
{
	//if (Owner) Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
	OnOpenRequest.Broadcast();
}

void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0.0f, 0, 0.0f));
}

