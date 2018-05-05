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

#if WITH_EDITOR
void AGridObject::PostEditMove(bool bFinished)
{
	if (bFinished)
	{
		UpdatePosition();
	}
	
	AActor::PostEditMove(bFinished);
}
#endif

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
		loc = loc / 100.0f;
		FIntVector pos = FIntVector(FMath::FloorToInt(loc.X), FMath::FloorToInt(loc.Y), FMath::FloorToInt(loc.Z));
		SetActorLocation(FVector(pos * 100) + FVector(50.0f, 50.0f, 0.0f));
		GridOrigin.Origin = pos;
		float x = (float)((int)GetActorRotation().Yaw % 360);
		float angle = x / 90.0f;
		auto rot = FRotator(0.0f, FMath::RoundToFloat( angle) * 90.0f, 0.0f);
		SetActorRotation(rot);
		auto vec = rot.RotateVector(FVector(0.0f, 1.0f, 0.0f));
		GridOrigin.Direction = FIntVector(
			FMath::RoundToInt(vec.X),
			FMath::RoundToInt(vec.Y),
			FMath::RoundToInt(vec.Z)
		);

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
