// Fill out your copyright notice in the Description page of Project Settings.

#include "MCommand.h"
#include "Unit.h"
#include "EngineUtils.h"
#include "UnrealNetwork.h"

void UMCommand::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMCommand, unit);
}

void UMCommand::Run()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Run"));

	unit->MoveTo(path);
}
