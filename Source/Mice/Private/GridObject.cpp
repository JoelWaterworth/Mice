// Fill out your copyright notice in the Description page of Project Settings.

#include "GridObject.h"
#include "WorldGrid.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGridObject::AGridObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void AGridObject::PostEditMove(bool bFinished)
{
	if (bFinished)
	{
		UpdatePosition();
	}
	
	AActor::PostEditMove(bFinished);
}

FGridObjectTree AGridObject::GetObjectTree()
{
	FGridObjectTree tree;
	TArray<UChildActorComponent*> Comps;
	GetComponents(Comps);
	tree.Trans = GridOrigin;

	for (UChildActorComponent* child : Comps)
	{
		AGridObject* object = Cast<AGridObject>(child->GetChildActor());
		if (object)
		{
			tree.Children.Add(object->GetObjectTree());
		}
	};
	return tree;
}

void AGridObject::UpdatePosition()
{
	if (RootComponent != nullptr && GetWorld() != nullptr)
	{
		auto loc = GetActorLocation();

		TArray<AActor*> worldGrids;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWorldGrid::StaticClass(), worldGrids);
		if (worldGrids.Num() == 0)
		{
			return;
		};
		AWorldGrid* worldGrid = Cast<AWorldGrid>(worldGrids[0]);
		if (worldGrid)
		{
			FIntVector offset = worldGrid->LocationToVector(loc);
			FRotator rotOffset = worldGrid->GetActorRotation();

			RootComponent->SetWorldLocation(worldGrid->VectorToWorldTransform(offset).GetLocation());
			GridOrigin.Origin = offset;
		}
		int x = (float)((int)GetActorRotation().Yaw % 360);
		int angle = (int)round((x < 0.0f ? x + 360.0f : x) / 90.f);
		switch (angle)
		{
		case 0:
			GridOrigin.Direction = EDirection::D_Forward;
			break;
		case 1:
			GridOrigin.Direction = EDirection::D_Rightward;
			break;
		case 2:
			GridOrigin.Direction = EDirection::D_Backward;
			break;
		case 3:
			GridOrigin.Direction = EDirection::D_Leftward;
			break;
		default:
			break;
		}
		SetActorRotation(FRotator(0.0f, (float)angle * 90.0f, 0.0f));

		TArray<UActorComponent*> actors = TArray<UActorComponent*>();
		GetComponents<UActorComponent>(actors);
		for (auto* actor : actors)
		{
			AGridObject* gridObject = Cast<AGridObject>(actor);
			if (gridObject)
			{
				gridObject->UpdatePosition();
			}
		}
	}
}
