// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldGrid.h"
#include "Algo/Reverse.h"
#include "MGameInstance.h"
#include "EngineUtils.h"
#include "../Public/WorldGrid.h"

DEFINE_LOG_CATEGORY(LogWorld);
#define LOCTEXT_NAMESPACE "World grid" 

// Sets default values
AWorldGrid::AWorldGrid()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	InstanceMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstanceMesh"));
	InstanceMesh->SetupAttachment(root);

	width = 10;
	height = 10;
	spacing = 100.0f;
	
	CollisionExtent = FVector(55.0f, 55.0f, 10.0f);
}

// Called when the game starts or when spawned
void AWorldGrid::BeginPlay()
{
	Super::BeginPlay();

}

TArray<FIntVector> AWorldGrid::PlotPine(FIntVector v0, FIntVector v1)
{
	TArray<FIntVector> line = TArray<FIntVector>();
	int32 i = 0;
	FIntVector step = FIntVector(0, 0, 0);
	int32 error_0, errorprev_0, error_1, errorprev_1 = 0;
	FIntVector vec = v0;
	int32 ddy, ddx, ddz;
	FIntVector d = v1 - v0;
	line.Add(vec);

	SetDeltaNStep(d.Y, step.Y);
	SetDeltaNStep(d.X, step.X);
	SetDeltaNStep(d.Z, step.Z);
	UE_LOG(LogWorld, Warning, TEXT("step is %s"), *step.ToString());
	ddy = 2 * d.Y;
	ddx = 2 * d.X;
	ddz = 2 * d.Z;
	if (ddx >= ddy && ddx >= ddz)
	{
		error_0 = d.X;
		errorprev_0 = d.X;
		error_1 = d.X;
		errorprev_1 = d.X;
		for(i = 0; i < d.X; i++) {
			vec.X += step.X;
			error_0 += ddy;
			error_1 += ddz;
			if (error_0 > ddx && error_1 > ddx)
			{
				vec.Y += step.Y;
				vec.Z += step.Z;
				error_0 -= ddx;
				error_1 -= ddx;
				if (error_0 + errorprev_0 < ddx){
					line.Add(FIntVector(vec.X, vec.Y - step.Y, vec.Z - step.Z));
				}
				else if (error_0 + errorprev_0 > ddx)
				{
					line.Add(FIntVector(vec.X - step.X, vec.Y, vec.Z));
				}
				else {
					line.Add(FIntVector(vec.X, vec.Y - step.Y, vec.Z));
					line.Add(FIntVector(vec.X - step.X, vec.Y, vec.Z));
					line.Add(FIntVector(vec.X, vec.Y, vec.Z - step.Z));
				}
			}
			else if (error_0 > ddx)
			{
				vec.Y += step.Y;
				error_0 -= ddx;
				if (error_0 + errorprev_0 < ddx)
				{
					line.Add(FIntVector(vec.X, vec.Y - step.Y, vec.Z));
				}
				else if (error_0 + errorprev_0 > ddx)
				{
					line.Add(FIntVector(vec.X - step.X, vec.Y, vec.Z));
				}
				else {
					line.Add(FIntVector(vec.X, vec.Y - step.Y, vec.Z));
					line.Add(FIntVector(vec.X - step.X, vec.Y, vec.Z));
				}
			}
			else if (error_1 > ddx)
			{
				vec.Z += step.Z;
				error_1 -= ddx;
				if (error_1 + errorprev_1 < ddx)
				{
					line.Add(FIntVector(vec.X, vec.Y, vec.Z - step.Z));
				}
				else if (error_1 + errorprev_1 > ddx)
				{
					line.Add(FIntVector(vec.X - step.X, vec.Y, vec.Z));
				}
				else
				{
					line.Add(FIntVector(vec.X, vec.Y, vec.Z - step.Z));
					line.Add(FIntVector(vec.X - step.X, vec.Y, vec.Z));
				}
			}
			errorprev_0 = error_0;
			errorprev_1 = error_1;
		}
	}
	else if (ddy >= ddz)
	{
		error_0 = d.Y;
		errorprev_0 = d.Y;
		error_1 = d.Y;
		errorprev_1 = d.Y;
		for (i = 0; i < d.Y; i++) {
			vec.Y += step.Y;
			error_0 += ddx;
			error_1 += ddz;
			if (error_0 > ddy && error_1 > ddy)
			{
				vec.X += step.X;
				vec.Z += step.Z;
				error_0 -= ddy;
				error_1 -= ddy;
				if (error_0 + errorprev_0 < ddy) {
					line.Add(FIntVector(vec.X - step.X, vec.Y, vec.Z));
				}
				else if (error_0 + errorprev_0 > ddy) {
					line.Add(FIntVector(vec.X, vec.Y - step.Y, vec.Z));
				}
				else {
					line.Add(FIntVector(vec.X - step.X, vec.Y, vec.Z));
					line.Add(FIntVector(vec.X, vec.Y - step.Y, vec.Z));
				}
			}
			else if (error_0 > ddy)
			{
				vec.X += step.X;
				error_0 -= ddx;
				if (error_0 + errorprev_0 < ddx)
				{
					line.Add(FIntVector(vec.X, vec.Y - step.Y, vec.Z));
				}
				else if (error_0 + errorprev_0 > ddx)
				{
					line.Add(FIntVector(vec.X - step.X, vec.Y, vec.Z));
				}
				else {
					line.Add(FIntVector(vec.X, vec.Y - step.Y, vec.Z));
					line.Add(FIntVector(vec.X - step.X, vec.Y, vec.Z));
				}
			}
			else if (error_1 > ddy)
			{
				vec.Z += step.Z;
				error_1 -= ddx;
				if (error_1 + errorprev_1 < ddx)
				{
					line.Add(FIntVector(vec.X, vec.Y, vec.Z - step.Z));
				}
				else if (error_1 + errorprev_1 > ddx)
				{
					line.Add(FIntVector(vec.X - step.X, vec.Y, vec.Z));
				}
				else
				{
					line.Add(FIntVector(vec.X, vec.Y, vec.Z - step.Z));
					line.Add(FIntVector(vec.X - step.X, vec.Y, vec.Z));
				}
			}
			errorprev_0 = error_0;
		}
	}
	else
	{
		error_0 = d.Z;
		errorprev_0 = d.Z;
		error_1 = d.Z;
		errorprev_1 = d.Z;
		for (i = 0; i < d.Y; i++) {
			vec.Z += step.Z;
			error_0 += ddy;
			error_1 += ddx;
			if (error_0 > ddz && error_1 > ddz)
			{
				vec.Y += step.Y;
				vec.X += step.X;
				error_0 -= ddx;
				error_1 -= ddx;
				if (error_0 + errorprev_0 < ddz)
				{
					line.Add(FIntVector(vec.X, vec.Y - step.Y, vec.Z - step.Z));
				}
				else if (error_0 + errorprev_0 > ddz)
				{
					line.Add(FIntVector(vec.X - step.X, vec.Y, vec.Z));
				}
				else {
					line.Add(FIntVector(vec.X, vec.Y - step.Y, vec.Z));
					line.Add(FIntVector(vec.X - step.X, vec.Y, vec.Z));
					line.Add(FIntVector(vec.X, vec.Y, vec.Z - step.Z));
				}
			}
			else if (error_0 > ddz)
			{
				vec.Y += step.Y;
				error_0 -= ddx;
				if (error_0 + errorprev_0 < ddz)
				{
					line.Add(FIntVector(vec.X, vec.Y - step.Y, vec.Z));
				}
				else if (error_0 + errorprev_0 > ddz)
				{
					line.Add(FIntVector(vec.X - step.X, vec.Y, vec.Z));
				}
				else
				{
					line.Add(FIntVector(vec.X, vec.Y - step.Y, vec.Z));
					line.Add(FIntVector(vec.X - step.X, vec.Y, vec.Z));
				}
			}
			else if (error_1 > ddz)
			{
				vec.X += step.X;
				error_1 -= ddz;
				if (error_1 + errorprev_1 < ddz)
				{
					line.Add(FIntVector(vec.X, vec.Y, vec.Z - step.Z));
				}
				else if (error_1 + errorprev_1 > ddz)
				{
					line.Add(FIntVector(vec.X - step.X, vec.Y, vec.Z));
				}
				else {
					line.Add(FIntVector(vec.X, vec.Y, vec.Z - step.Z));
					line.Add(FIntVector(vec.X - step.X, vec.Y, vec.Z));
				}
			}
			errorprev_0 = error_0;
			errorprev_1 = error_1;
			}
		}
	return line;
}

// Called every frame
void AWorldGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FTransform AWorldGrid::VectorToWorldTransform(FIntVector pos)
{
	return VectorToLocalTransform(pos) * GetActorTransform();
}

FTransform AWorldGrid::VectorToLocalTransform(FIntVector pos)
{
	FIntVector nPos = (pos * spacing) + FIntVector(spacing /2, spacing / 2, 0);
	return FTransform(FVector((float)nPos.X, (float)nPos.Y, (float)nPos.Z));
}

FIntVector AWorldGrid::LocationToVector(FVector currentPos)
{
	FVector RelativePos = currentPos - GetActorLocation();
	FIntVector pos = FIntVector();
	pos.X = (int32)FMath::RoundHalfToZero(RelativePos.X / spacing);
	pos.Y = (int32)FMath::RoundHalfToZero(RelativePos.Y / spacing);
	pos.Z = (int32)FMath::RoundHalfToZero(RelativePos.Z / spacing);
	return pos;
}

TMap<FIntVector, FIntVector> AWorldGrid::CalculatePaths(AUnit * Unit, int32 Limit)
{
	TArray<FIntVector> OpenSet = TArray<FIntVector>();
	OpenSet.Add(Unit->pos);

	TArray<FIntVector> CloseSet = TArray<FIntVector>();

	TMap<FIntVector, int32> gScore = TMap<FIntVector, int32>();
	gScore.Add(Unit->pos, 0);
	TMap<FIntVector, FIntVector> roots = TMap<FIntVector, FIntVector>();
	while (OpenSet.Num() > 0)
	{
		FIntVector current = OpenSet[0];
		OpenSet.RemoveAt(0);
		CloseSet.Add(current);

		TArray<FIntVector> neighbours = GetNeighbours(current);

		for (FIntVector& neighbour : neighbours)
		{
			if (!CloseSet.Contains(neighbour)) 
			{
				if (!OpenSet.Contains(neighbour)) 
				{
					OpenSet.Add(neighbour);
				}
				int32* y = gScore.Find(current);
				if (y) {
					int32 currentScore = *y + 1;

					int32* x = gScore.Find(neighbour);

					if (x)
					{
						if ((currentScore < Limit) && (currentScore <  *x))
						{
							gScore.Add(neighbour, currentScore);
							roots.Add(neighbour, current);
						}
					}
					else if (currentScore < Limit)
					{
						gScore.Add(neighbour, currentScore);
						roots.Add(neighbour, current);
					}
				}
				else {
					UE_LOG(LogTemp, Warning, TEXT("failed to find %d: %d"), current.X, current.Y);
				}
			}
		}
	}

	DebugPath(gScore);

	return roots;
}

TArray<FIntVector> AWorldGrid::CreatePathFromRoutes(TMap<FIntVector, FIntVector> cameFrom, FIntVector Dest)
{
	FIntVector* current = &Dest;
	TArray<FIntVector> path = TArray<FIntVector>();
	path.Add(*current);
	while (cameFrom.Contains(*current))
	{
		current = cameFrom.Find(*current);
		if (current) {
			path.Add(*current);
		}
		else
		{
			break;
		}
	}
	path.Pop();
	Algo::Reverse(path);
	return path;
}

void AWorldGrid::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	//Empty the array and delete all it's components
	
	if (GetWorld()->GetGameInstance())
	{
		UMGameInstance* GameInstance = Cast<UMGameInstance>(GetWorld()->GetGameInstance());
		if (GameInstance)
		{
			spacing = GameInstance->Spacing;
		}
	}

	TArray<UBoxComponent*> boxs;
	GetComponents(boxs);

	for (UBoxComponent* box : boxs)
	{
		if (box)
		{
			box->DestroyComponent();
		}
	}
	/*
	for (USceneComponent* elem : waste)
	{
		if (elem)
		{
			elem->DestroyComponent();
		}
	}
	*/

	//waste.Empty();
	gridTiles.Empty();

	TArray<FGridTransform> trans;
	TSet<FIntVector> nonTiles;

	for (TActorIterator<AGridObject> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Access the subclass instance with the * or -> operators.
		AGridObject* object = *ActorItr;
		trans.Add(object->GridOrigin);
		for (FIntVector& pos : object->GridOrigin.BlockedTiles) {
			nonTiles.Add(pos + object->GridOrigin.Origin);
		}
	}

	for (FGridTransform& tran : trans)
	{
		for (FIntVector& pos : tran.WalkablePosistions)
		{
			if (!nonTiles.Contains(pos)) {
				FIntVector loc = pos + tran.Origin;
				int32 x = loc.X, y = loc.Y, z = loc.Z;
				FVector vec = FVector((float)(x * spacing), (float)(y * spacing), (float)(z * spacing)) + FVector(spacing / 2, spacing / 2, 0.0f);

				UGridCollision* col = NewObject<UGridCollision>(this, UGridCollision::StaticClass());
				if (col) {
					col->RegisterComponent();
					waste.Add(col);
					col->SetBoxExtent(FVector(spacing / 2, spacing / 2, 10.0f), true);
					col->pos = FIntVector(x, y, 0);
					col->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
					col->AttachToComponent(root, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
					FTransform colTrans = FTransform(FRotator(0.0f), vec);
					col->SetRelativeTransform(colTrans);
				}
				gridTiles.Add(FIntVector(x, y, z), FGridTile(true, col));
			}
		}
	}

	Refresh = false;
	//Register all the components
	RegisterAllComponents();
}

void AWorldGrid::DebugPath(TMap<FIntVector, int32> gScore)
{
	for (TPair<FIntVector, FGridTile>& tile : gridTiles)
	{
		if (tile.Value.DebugTextRender)
		{
			tile.Value.DebugTextRender->SetText(LOCTEXT("","x"));
		}
	}

	for (TPair<FIntVector, int32>& elem : gScore)
	{
		FGridTile* tile = gridTiles.Find(elem.Key);
		if (tile)
		{
			if (tile->DebugTextRender)
			{
				tile->DebugTextRender->SetText(FText::FromString(FString::FromInt(elem.Value)));
			}
		}
	}
}

TArray<FIntVector> AWorldGrid::GetNeighbours(FIntVector origin)
{
	TArray<FIntVector> neighbours = TArray<FIntVector>();

	for (int32 x = -1; x < 2; x++)
	{
		for (int32 y = -1; y < 2; y++)
		{
			for (int32 z = -1; z < 2; z++)
			{
				if (!(x == 0 && y == 0))
				{
					FIntVector pos = FIntVector(x, y, z) + origin;
					if (gridTiles.Contains(pos))
					{
						neighbours.Add(pos);
					}
				}
			}
		}
	}
	return neighbours;
}

void AWorldGrid::SetDeltaNStep(int32 & delta, int32 & step)
{
	if (delta < 0)
	{
		step = -1;
		delta = -delta;
	}
	else 
	{
		step = 1;
	}
}

#undef LOCTEXT_NAMESPACE 