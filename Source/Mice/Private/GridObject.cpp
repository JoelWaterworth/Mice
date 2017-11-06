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
	
	if (bFinished && RootComponent != nullptr && GetWorld() != nullptr)
	{
		auto loc = GetActorLocation();

		TArray<AActor*> worldGrids;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWorldGrid::StaticClass(), worldGrids);
		AWorldGrid* worldGrid = Cast<AWorldGrid>(worldGrids[0]);
		if (worldGrid)
		{
			FIntVector offset = worldGrid->LocationToVector(loc);
			FRotator rotOffset = worldGrid->GetActorRotation();

			RootComponent->SetWorldLocation(worldGrid->VectorToWorldTransform(offset).GetLocation());
			GridOrigin.Origin = offset;
		}
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
