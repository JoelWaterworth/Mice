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

	width = 10;
	height = 10;
	spacing = 120;

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			FTransform trans = FTransform(FVector((float)(x * spacing), (float)(y * spacing), 0.0f));
			InstanceMesh->AddInstance(trans);
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

void AWorldGrid::OnConstruction(const FTransform & Transform)
{
	for (auto& com : waste) {
		com->DestroyComponent(true);
		waste.Remove(com);
	}

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			UTextRenderComponent* text = NewObject<UTextRenderComponent>(this, UTextRenderComponent::StaticClass());
			FTransform textTrans = FTransform(FRotator(0.0f, 90.f, 0.0f), FVector((float)(width * spacing), (float)(height * spacing), 10.0f), FVector());
			text->SetRelativeTransform(textTrans);
			waste.Add(text);

			UGridCollision* col = NewObject<UGridCollision>(this, UGridCollision::StaticClass());
			FTransform colTrans = FTransform(FRotator(), FVector((float)(width * spacing), (float)(height * spacing), 0.0f), FVector());
			col->SetRelativeTransform(colTrans);
			waste.Add(col);
			col->SetBoxExtent(CollisionExtent, true);
			col->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block)
		}
	}
}

