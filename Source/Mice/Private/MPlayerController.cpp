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
}

bool AMPlayerController::ServerSubmitCommands_Validate()
{
	return true;
}

void AMPlayerController::ServerSubmitCommands_Implementation()
{
	AMGameState* gameState = Cast<AMGameState>(GetWorld()->GetGameState());
	gameState->SumbitCommands(this);
}

void AMPlayerController::UpdatePlayerTeam_Implementation(ETeam team)
{

}

void AMPlayerController::BeginPlay()
{
	bShowMouseCursor = true;
	Super::BeginPlay();
}

TArray<UMCommand*> AMPlayerController::GetCommands()
{
	return commands;
}
