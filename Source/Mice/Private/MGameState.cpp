// Fill out your copyright notice in the Description page of Project Settings.

#include "MGameState.h"
#include "UnrealNetwork.h"
#include "EngineUtils.h"
#include "Engine.h"
#include "Unit.h"
#include "MPlayerState.h"

AMGameState::AMGameState()
{
	isBlueReady = false;
	isRedReady = false;
}
void AMGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMGameState, blueCommands);
	DOREPLIFETIME(AMGameState, redCommands);
	DOREPLIFETIME(AMGameState, isBlueReady);
	DOREPLIFETIME(AMGameState, isRedReady);
}

void AMGameState::SumbitCommand_Implementation(AMPlayerController* playerController, AUnit* unit){}

void AMGameState::SumbitCommands_Implementation(AMPlayerController * playerController)
{
	if (playerController)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("submit called"));
		AMPlayerState* playerState = Cast<AMPlayerState>(playerController->PlayerState);

		if (playerState->team == ETeam::T_Blue)
		{
			blueCommands = playerController->GetCommands();
			isBlueReady = true;
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("blue has submit calls"));
		}

		if (playerState->team == ETeam::T_Red)
		{
			redCommands = playerController->GetCommands();
			isRedReady = true;
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("red has submit calls"));
		}

		if (isBlueReady && isRedReady)
		{
			ExecuteCommands(redCommands);
			ExecuteCommands(blueCommands);
			redCommands.Empty();
			blueCommands.Empty();
			isBlueReady = false;
			isRedReady = false;
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("execute commands"));
		}
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("player controller is invalid"));
	}
}

void AMGameState::ExecuteCommands(TMap<AUnit*, UMCommand*> commands)
{
	for (auto& command : commands)
	{
		if (command.Value)
		{
			command.Value->Run(command.Key);
		}
		else
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("invalid command"));
		}
	}
}

void AMGameState::ExecuteCommand(UMCommand* command, AUnit* target)
{
	command->Run(target);
}