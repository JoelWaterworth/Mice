// Fill out your copyright notice in the Description page of Project Settings.

#include "MPlayerController.h"
#include "Public/WorldGrid.h"
#include "Kismet/GameplayStatics.h"
#include "MGameState.h"
#include "MPlayerState.h"
#include "UnrealNetwork.h"
#include "../Public/MPlayerController.h"

AMPlayerController::AMPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMPlayerController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMPlayerController, commands);
	//DOREPLIFETIME(AMPlayerController, CommandMap);
}

bool AMPlayerController::ServerSubmitCommands_Validate()
{
	return true;
}

void AMPlayerController::ServerSubmitCommands_Implementation()
{
	AMGameState* gameState = Cast<AMGameState>(GetWorld()->GetGameState());
	if (gameState)
	{
		gameState->SumbitCommands(this);
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("gamestate is invalid"));
	}
}

bool AMPlayerController::RunCommand_Validate(UMCommand* command, AUnit* unit)
{
	return true;
}

void AMPlayerController::RunCommand_Implementation(UMCommand* command, AUnit* unit)
{
	AMGameState* gameState = Cast<AMGameState>(GetWorld()->GetGameState());
	if (gameState)
	{
		gameState->ExecuteCommand(command, unit);
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("gamestate is invalid"));
	}
}

void AMPlayerController::UpdatePlayerTeam_Implementation(ETeam team)
{

}

void AMPlayerController::ClientPlayerUpdate_Implementation(ETeam team)
{
	UpdatePlayerTeam(team);
}

void AMPlayerController::BeginPlay()
{
	bShowMouseCursor = true;
	Super::BeginPlay();
}