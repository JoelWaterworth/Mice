// Fill out your copyright notice in the Description page of Project Settings.

#include "MPlayerController.h"
#include "Public/WorldGrid.h"
#include "Kismet/GameplayStatics.h"
#include "../Public/MPlayerController.h"

AMPlayerController::AMPlayerController()
{

}

void AMPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Target", IE_Pressed, this, &AMPlayerController::Target);
	InputComponent->BindAction("Select", IE_Pressed, this, &AMPlayerController::Select);
}

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
				Selected->MoveTo(RootsToPath(col->pos, roots));
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
			if (worldGrid) {
				roots = worldGrid->CalculatePaths(Selected, 8);
			}
		}
	}
}

TArray<FVector2DInt> AMPlayerController::RootsToPath(FVector2DInt dest, TMap<FVector2DInt, FVector2DInt> roots)
{
	FVector2DInt* current = &dest;
	TArray<FVector2DInt> path = TArray<FVector2DInt>();
	path.Add(*current);
	while (roots.Contains(*current))
	{
		current = roots.Find(*current);
		if (current) {
			path.Add(*current);
		}
		else {
			break;
		}
	}
	path.Pop();
	Algo::Reverse(path);
	return path;
}

void AMPlayerController::BeginPlay()
{
	Super::BeginPlay();
	bShowMouseCursor = true;

	UWorld* const World = GetWorld();
	if (World)
	{
		TArray<AActor*> worldGrids;
		UGameplayStatics::GetAllActorsOfClass(World, AWorldGrid::StaticClass(), worldGrids);
		AWorldGrid* worldGrid = Cast<AWorldGrid>(worldGrids[0]);

		FTransform trans = worldGrid->VectorToWorldTransform(FVector2DInt(0,0));
		Cursor = World->SpawnActor<AActor>(CursorClass, trans);
	};
}

void AMPlayerController::Tick(float DeltaTime)
{
	FHitResult Res;
	bool Hit = GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1), true, Res);
	if (Hit)
	{
		UGridCollision* col = Cast<UGridCollision>(Res.GetComponent());
		if (col)
		{
			AWorldGrid* grid = Cast<AWorldGrid>(Res.GetActor());
			if (grid)
			{
				Cursor->SetActorLocation( grid->VectorToWorldTransform(col->pos).GetLocation() + FVector(0.0f, 0.0f, 10.0f));
			}
		}
	}
}
