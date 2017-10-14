// Fill out your copyright notice in the Description page of Project Settings.

#include "Unit.h"
#include "Classes/Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
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

	//bReplicates = true;
}

void AUnit::MoveTo(TArray<FVector2DInt> path)
{
	Pathpos = 0;
	State = EUnitState::US_Walk;

	Path = path;
	SetNewLocation();
}

// Called when the game starts or when spawned
void AUnit::BeginPlay()
{
	Super::BeginPlay();
	
}

void AUnit::SetNewLocation()
{
	TArray<AActor*> worldGrids;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWorldGrid::StaticClass(), worldGrids);
	AWorldGrid* worldGrid = Cast<AWorldGrid>(worldGrids[0]);

	NextLocation = worldGrid->VectorToWorldTransform(Path[Pathpos]).GetLocation();
}

// Called every frame
void AUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (State)
	{
	case EUnitState::US_Idle :
		break;
	case EUnitState::US_Walk :
		{
			FVector vec = NextLocation - GetActorLocation();
			float distToTravel = vec.Size();
			vec.Normalize();
			vec = vec * (100.0f * DeltaTime);
			float delta = vec.Size();
			if ( distToTravel < delta )
			{
				SetActorLocation(NextLocation);
				pos = Path[Pathpos];
				Pathpos += 1;
				if (Pathpos >= Path.Num())
				{
					State = EUnitState::US_Idle;
				}
				else {
					SetNewLocation();
				}
			}
			else {
				SetActorLocation(vec + GetActorLocation());
			}
		}
	}
}

