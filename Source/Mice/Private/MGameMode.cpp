// Fill out your copyright notice in the Description page of Project Settings.

#include "MGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "PlayerCamera.h"
#include "WorldGrid.h"
#include "MPlayerStart.h"
#include "MPlayerState.h"
#include "Classes/Engine/World.h"
#include "Unit.h"
#include "EngineUtils.h"
#include "UnitSpawn.h"
#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>
#include "Kismet/GameplayStatics.h"


AMGameMode::AMGameMode()
{
	DefaultPawnClass = APlayerCamera::StaticClass();
	PlayerStateClass = AMPlayerState::StaticClass();
}

void AMGameMode::PostLogin(APlayerController * NewPlayer)
{
	// Place player on a team before Super (VoIP team based init, findplayerstart, etc)
	AMPlayerState* NewPlayerState = Cast<AMPlayerState>(NewPlayer->PlayerState);
	ETeam team = ChooseTeam();
	UE_LOG(LogTemp, Warning, TEXT("PlayerArray %d"), (int)team);
//	NewPlayerState->SetTeam(team);
	Super::PostLogin(NewPlayer);
}

void AMGameMode::BeginPlay()
{
	Super::BeginPlay();

	UWorld* const World = GetWorld();
	if (World)
	{
		TArray<AActor*> worldGrids;
		UGameplayStatics::GetAllActorsOfClass(World, AWorldGrid::StaticClass(), worldGrids);
		AWorldGrid* worldGrid = Cast<AWorldGrid>(worldGrids[0]);
		if (worldGrid) {
			if ((worldGrid->BlueSpawnPoints.Num() > 0) && (worldGrid->RedSpawnPoints.Num() > 0))
			{
				for (AUnitSpawn* spawn : worldGrid->SpawnPoints)
				{
					FTransform trans = FTransform(FVector(spawn->origin * 100.0f) + FVector(50.0f, 50.0f, 0.0f));
					AUnit* unit = World->SpawnActor<AUnit>(worldGrid->spawnClass, trans);
					if (unit)
					{
						unit->UpdatePos(spawn->origin);
						unit->ServerUpdateTeam(spawn->team);
						unit->SetActorRotation(spawn->GetActorRotation());
					}
				}
			}
		}
	};
}

ETeam AMGameMode::ChooseTeam()
{
	int32 count = GameState->PlayerArray.Num();
	if (count % 2 == 1) {
		return ETeam::T_Blue;
	}
	else {
		return ETeam::T_Red;
	}
}

AActor * AMGameMode::ChoosePlayerStart_Implementation(AController * Player)
{
	AMPlayerState* playerState = Cast<AMPlayerState>(Player->PlayerState);

	UWorld* const World = GetWorld();
	if (World)
	{
		for (TActorIterator<AMPlayerStart> playerStart(World); playerStart; ++playerStart)
		{
			AMPlayerStart* start = *playerStart;
			if (!start->inUse)
			{
				playerState->SetTeam(start->team);
				start->inUse = true;
				return start;
			}
		}
	}
	return nullptr;

}