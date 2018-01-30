// Fill out your copyright notice in the Description page of Project Settings.

#include "UnitSpawn.h"
#include "WorldGrid.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AUnitSpawn::AUnitSpawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}
#if WITH_EDITOR
void AUnitSpawn::PostEditMove(bool bFinished)
{
	if (bFinished && RootComponent != nullptr && GetWorld() != nullptr)
	{
		auto loc = GetActorLocation();

		TArray<AActor*> worldGrids;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWorldGrid::StaticClass(), worldGrids);
		if (worldGrids.Num() == 0) {
			AActor::PostEditMove(bFinished);
			return;
		};
		AWorldGrid* worldGrid = Cast<AWorldGrid>(worldGrids[0]);
		if (worldGrid)
		{
			FIntVector offset = worldGrid->LocationToVector(loc);
			FRotator rotOffset = worldGrid->GetActorRotation();

			RootComponent->SetWorldLocation(worldGrid->VectorToWorldTransform(offset).GetLocation());
			origin = offset;
			worldGrid->GetSpawnPoints();
		}
	}

	AActor::PostEditMove(bFinished);
}
#endif