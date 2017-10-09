// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldGrid.h"


// Sets default values
AWorldGrid::AWorldGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	InstanceMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstanceMesh"));
	InstanceMesh->SetupAttachment(root);

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			InstanceMesh->Add();
			USceneComponent* baseScene = NewObject<USceneComponent>(this, USceneComponent::StaticClass());
		}
	}

}

// Called when the game starts or when spawned
void AWorldGrid::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWorldGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

