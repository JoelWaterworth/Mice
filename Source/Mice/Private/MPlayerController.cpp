// Fill out your copyright notice in the Description page of Project Settings.

#include "MPlayerController.h"
#include "Public/WorldGrid.h"
#include "Kismet/GameplayStatics.h"
#include "MGameState.h"
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

void AMPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//InputComponent->BindAction("Target", IE_Pressed, this, &AMPlayerController::Target);
	//InputComponent->BindAction("Select", IE_Pressed, this, &AMPlayerController::Select);
}
/*
void AMPlayerController::Target()
{
	FHitResult Res;
	bool Hit = GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1), true, Res);
	if (Hit)
	{
		if (Selected)
		{
			UGridCollision* col = Cast<UGridCollision>(Res.GetComponent());
			if (col)
			{
				Selected->WalkCommand(col->pos, this);
				if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Add command"));
			}
		}
	}
}

void AMPlayerController::Select()
{
	FHitResult Res;
	bool Hit = GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1), true, Res);
	if (Hit)
	{
		AUnit* unit = Cast<AUnit>(Res.GetActor());
		if (unit)
		{
			Selected = unit;
			Selected->selectPlane->SetVisibility(true);
			TArray<AActor*> worldGrids;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWorldGrid::StaticClass(), worldGrids);
			AWorldGrid* worldGrid = Cast<AWorldGrid>(worldGrids[0]);
			if (worldGrid)
			{
				unit->Paths = worldGrid->CalculatePaths(Selected, 8);
			}
		}
	}
}
*/
void AMPlayerController::BeginPlay()
{
	bShowMouseCursor = true;
	Super::BeginPlay();
}

TArray<UMCommand*> AMPlayerController::GetCommands()
{
	return commands;
}
