// Fill out your copyright notice in the Description page of Project Settings.

#include "MCommand.h"
#include "Unit.h"
#include "EngineUtils.h"
#include "UnrealNetwork.h"

void UMCommand::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMCommand, unitID);
}

void UMCommand::Run()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Run"));

	UWorld* const World = GetWorld();
	if (World)
	{
		for (TActorIterator<AUnit> unit(World); unit; ++unit)
		{
			if (unit->GetUniqueID() == unitID)
			{
				unit->MoveTo(path);
			}
		}
	}
}
