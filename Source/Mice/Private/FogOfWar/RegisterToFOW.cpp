// Fill out your copyright notice in the Description page of Project Settings.

#include "RegisterToFOW.h"
#include "FogOfWarManager.h"
#include "EngineUtils.h"

// Sets default values for this component's properties
URegisterToFOW::URegisterToFOW()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void URegisterToFOW::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<AFogOfWarManager> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Access the subclass instance with the * or -> operators.
		AFogOfWarManager* object = *ActorItr;
		if (object) {
			Manager = object;
		}
	}

	FString ObjectName = GetOwner()->GetName();
	UE_LOG(LogTemp, Log, TEXT("I am alive %s"), *ObjectName);

	//registering the actor to the FOW Manager

	if (Manager != nullptr) {
		UE_LOG(LogTemp, Log, TEXT("Found Manager"));

		//Manager->RegisterFowActor(GetOwner());
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Please attach a FOW Manager"));
	}

}


// Called every frame
void URegisterToFOW::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

