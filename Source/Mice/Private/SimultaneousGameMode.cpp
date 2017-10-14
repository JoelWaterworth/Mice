// Fill out your copyright notice in the Description page of Project Settings.

#include "SimultaneousGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "PlayerCamera.h"
#include "WorldGrid.h"
#include "MPlayerStart.h"
#include "MPlayerState.h"
#include "Classes/Engine/World.h"
#include "Unit.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"


ASimultaneousGameMode::ASimultaneousGameMode()
{
	DefaultPawnClass = APlayerCamera::StaticClass();
	PlayerStateClass = AMPlayerState::StaticClass();

	isBlueReady = false;
	isRedReady = false;
}

void ASimultaneousGameMode::PostLogin(APlayerController * NewPlayer)
{
	// Place player on a team before Super (VoIP team based init, findplayerstart, etc)
	AMPlayerState* NewPlayerState = Cast<AMPlayerState>(NewPlayer->PlayerState);
	const ETeam TeamNum = ChooseTeam(NewPlayerState);
	NewPlayerState->team = TeamNum;

	Super::PostLogin(NewPlayer);
}

void ASimultaneousGameMode::sumbitCommands(AMPlayerController * playerController)
{
	AMPlayerState* playerState = Cast<AMPlayerState>(playerController->PlayerState);

	if (playerState->team == ETeam::T_Blue)
	{
		blueCommands = playerController->GetCommands();
		isBlueReady = true;
	}

	if (playerState->team == ETeam::T_Red)
	{
		redCommands = playerController->GetCommands();
		isRedReady = true;
	}

	ExecuteCommands();

	if (isBlueReady && isRedReady)
	{
		ExecuteCommands();
	}
}

void ASimultaneousGameMode::BeginPlay()
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
				for (FVector2DInt& pos : worldGrid->BlueSpawnPoints)
				{
					FTransform trans = worldGrid->VectorToWorldTransform(pos);
					AUnit* unit = World->SpawnActor<AUnit>(worldGrid->spawnClass, trans);
					if (unit)
					{
						unit->pos = pos;
					}
				}

				for (FVector2DInt& pos : worldGrid->RedSpawnPoints)
				{
					FTransform trans = worldGrid->VectorToWorldTransform(pos);
					AUnit* unit = World->SpawnActor<AUnit>(worldGrid->spawnClass, trans);
					if (unit)
					{
						unit->pos = pos;
					}
				}
			}
		}
		/*
		for (TActorIterator<AUnit> ActorItr(World); ActorItr; ++ActorItr)
		{
			TArray<FVector2DInt> path = TArray<FVector2DInt>();
			path.Add(FVector2DInt(2, 2));
			ActorItr->MoveTo(path);
		}
		*/
	};
}

ETeam ASimultaneousGameMode::ChooseTeam(APlayerState * playerState)
{
	int32 count = GameState->PlayerArray.Num();
	if (count == 1) {
		return ETeam::T_Blue;
	}
	else {
		return ETeam::T_Red;
	}
	
}

AActor * ASimultaneousGameMode::ChoosePlayerStart_Implementation(AController * Player)
{
	AMPlayerState* playerState = Cast<AMPlayerState>(Player->PlayerState);
	ETeam playerTeam = playerState->team;

	UWorld* const World = GetWorld();
	if (World)
	{
		for (TActorIterator<AMPlayerStart> playerStart(World); playerStart; ++playerStart)
		{
			if (playerTeam == playerStart->team)
			{
				return *playerStart;
			}
		}
	}
	return nullptr;
}

void ASimultaneousGameMode::ExecuteCommands()
{
	for (UMCommand* command : blueCommands)
	{
		command->Run();
	}
	for (UMCommand* command : redCommands)
	{
		command->Run();
	}
	blueCommands.Empty();
	redCommands.Empty();
}
