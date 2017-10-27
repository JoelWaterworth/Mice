// Fill out your copyright notice in the Description page of Project Settings.

#include "MGameState.h"
#include "UnrealNetwork.h"
#include "EngineUtils.h"
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

void AMGameState::SumbitCommands(AMPlayerController * playerController)
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
			ExecuteCommands();
		}

		if (playerState->team == ETeam::T_Red)
		{
			redCommands = playerController->GetCommands();
			isRedReady = true;
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("red has submit calls"));
			ExecuteCommands();
		}

		if (isBlueReady && isRedReady)
		{
			ExecuteCommands();
		}
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("player controller is invalid"));
	}
}

void AMGameState::ExecuteCommands()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Execute Commands"));

	if ((blueCommands.Num() == 0) && (redCommands.Num() == 0))
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No Commands"));
	}
	for (UMCommand* command : blueCommands)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Execute blue"));
		command->Run();
	}
	for (UMCommand* command : redCommands)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Execute blue"));
		command->Run();
	}
	blueCommands.Empty();
	redCommands.Empty();
}
