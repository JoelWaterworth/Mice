// Fill out your copyright notice in the Description page of Project Settings.

#include "SimultaneousGameMode.h"
#include "PlayerCamera.h"
#include "WorldGrid.h"
#include "Unit.h"
#include "Kismet/GameplayStatics.h"


ASimultaneousGameMode::ASimultaneousGameMode()
{
	DefaultPawnClass = APlayerCamera::StaticClass();
}

void ASimultaneousGameMode::BeginPlay()
{
	UWorld* const World = GetWorld();
	if (World)
	{
		TArray<AActor*> worldGrids;
		UGameplayStatics::GetAllActorsOfClass(World, AWorldGrid::StaticClass(), worldGrids);
		AWorldGrid* worldGrid = Cast<AWorldGrid>(worldGrids[0]);
		if (worldGrid) {
			if ((worldGrid->BlueSpawnPoints.Num() > 0) && (worldGrid->RedSpawnPoints.Num() > 0))
			{
				FTransform trans = worldGrid->VectorToWorldTransform(worldGrid->BlueSpawnPoints[0]);
				AUnit* unit = World->SpawnActor<AUnit>(worldGrid->spawnClass, trans);
				if (unit)
				{
					unit->pos = worldGrid->BlueSpawnPoints[0];
				}
			}
		}

	};
}
