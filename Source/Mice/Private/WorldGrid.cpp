// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldGrid.h"
#include "Algo/Reverse.h"
#include "MGameInstance.h"
#include "EngineUtils.h"
#include "VoxelLineTraceIterator.h"
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

bool AWorldGrid::PlotLine(FVector s, FVector e)
{
	FVector start = s / 100.0f;
	FVector end = e / 100.0f;
	UE_LOG(LogWorld, Log, TEXT("number of obstructions %d"), obstucles.Num());
	UE_LOG(LogWorld, Log, TEXT("PlotLine start %s, end %s"),
		*start.ToString(), *end.ToString());
	FIntVector endtile = FIntVector(end);
	UVoxelLineTraceIterator* Component = NewObject<UVoxelLineTraceIterator>(this, UVoxelLineTraceIterator::StaticClass());
	Component->start = start;
	Component->direction = end - start;
	UE_LOG(LogWorld, Log, TEXT("direction %s and start %s"), *Component->direction.ToString(), *Component->start.ToString());
	for (int i = 0; i < 20; i++)
	{
		FIteratorReturn iter = Component->next();
		UE_LOG(LogWorld, Log, TEXT("start %s"), *Component->start.ToString());
		for (auto& voxel_res : iter.voxels)
		{
			UE_LOG(LogWorld, Log, TEXT("voxels %s"), *voxel_res.voxels.ToString());
			FObstucle* k = obstucles.Find(voxel_res.voxels);
			if (endtile == voxel_res.voxels)
			{
				return true;
			}
			else if (k != NULL)
			{
				return false;
			}
		}
	}
	return false;
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

	TMap<FIntVector, float> gScore = TMap<FIntVector, float>();
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
				float* y = gScore.Find(current);
				if (y) {
					float currentScore = *y + FVector::Dist(FVector(current), FVector(neighbour));
					float* x = gScore.Find(neighbour);

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
	for (USceneComponent* elem : waste)
	{
		if (elem)
		{
			elem->DestroyComponent();
		}
	}

	waste.Empty();
	gridTiles.Empty();
	obstucles.Empty();

	TArray<FGridTransform> trans;

	for (TActorIterator<AGridObject> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Access the subclass instance with the * or -> operators.
		AGridObject* object = *ActorItr;
		trans.Add(object->GridOrigin);
		for (FIntVector& pos : object->GridOrigin.BlockedTiles) {
			FIntVector loc = pos + object->GridOrigin.Origin;
			if (object->GridOrigin.isBorder) {
				auto boarder = [](FIntVector loc, EDirection dir, EDirection righthand,TMap<FBoarderKey, FObstucle>& WallObstucles, AGridObject* object) {
					WallObstucles.Add(FBoarderKey(dir, loc),
						FObstucle(object->GridOrigin.blockPercentage));
					if (object->GridOrigin.isRightHandCorner) {
						WallObstucles.Add(FBoarderKey(righthand, loc),
							FObstucle(object->GridOrigin.blockPercentage));
					};
				};
				switch (object->GridOrigin.Direction) {
					case EDirection::D_Forward :
						boarder(loc, EDirection::D_Forward, EDirection::D_Rightward, WallObstucles, object);
						break;
					case EDirection::D_Rightward:
						boarder(loc, EDirection::D_Rightward, EDirection::D_Backward, WallObstucles, object);
						break;
					case EDirection::D_Backward:
						boarder(loc, EDirection::D_Backward, EDirection::D_Leftward, WallObstucles, object);
						break;
					case EDirection::D_Leftward:
						boarder(loc, EDirection::D_Leftward, EDirection::D_Forward, WallObstucles, object);
						break;
				default:
					WallObstucles.Add(FBoarderKey(object->GridOrigin.Direction, loc),
						FObstucle(object->GridOrigin.blockPercentage));
					break;
				}
			}
			else
			{
				obstucles.Add(loc,
					FObstucle(object->GridOrigin.blockPercentage));
			}
		}
	}

	for (FGridTransform& tran : trans)
	{
		for (FIntVector& pos : tran.WalkablePosistions)
		{
			if (!obstucles.Contains(pos)) {
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

void AWorldGrid::DebugPath(TMap<FIntVector, float> gScore)
{
	/*
	for (TPair<FIntVector, FGridTile>& tile : gridTiles)
	{
		if (tile.Value.DebugTextRender)
		{
			tile.Value.DebugTextRender->SetText(LOCTEXT("","x"));
		}
	}

	for (TPair<FIntVector, float>& elem : gScore)
	{
		FGridTile* tile = gridTiles.Find(elem.Key);
		if (tile)
		{
			if (tile->DebugTextRender)
			{
				tile->DebugTextRender->SetText(FText::FromString(FString::SanitizeFloat(elem.Value)));
			}
		}
	}*/
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