// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit.h"
#include "Mice.h"
#include "Classes/Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MPlayerController.h"
#include "MCommand.h"
#include "Public/WorldGrid.h"
#include "../Public/Unit.h"

// Sets default values
AUnit::AUnit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	selectPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"));
	selectPlane->SetupAttachment(root);
	selectPlane->SetVisibility(false);

	bReplicates = true;
}

// Called when the game starts or when spawned
void AUnit::BeginPlay()
{
	Super::BeginPlay();
	
}