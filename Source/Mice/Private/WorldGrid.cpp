// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldGrid.h"
#include "Algo/Reverse.h"
#include "MGameInstance.h"
#include "EngineUtils.h"
#include "UnitSpawn.h"
#include "VoxelLineTraceIterator.h"
#include "../Public/WorldGrid.h"

DEFINE_LOG_CATEGORY(LogWorld);
#define LOCTEXT_NAMESPACE "World grid" 

// Sets default values
AWorldGrid::AWorldGrid()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	InstanceMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstanceMesh"));
	InstanceMesh->SetupAttachment(root);
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
		for (auto& voxel_res : iter.in_voxels)
		{
			UE_LOG(LogWorld, Log, TEXT("voxels %s"), *voxel_res.voxel.ToString());
			FObstucle* k = obstucles.Find(voxel_res.voxel);
			if (endtile == voxel_res.voxel)
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

float AWorldGrid::CalculateProbabilityOfShot(FVector start, FVector end, AUnit* unit)
{
	FIntVector endTile = FIntVector(end / 100.0f);
	FVector dir = (end - start) / 100.0f;
	float f = unit->weapon.projectProbArc->GetFloatValue(dir.Size());
	if (f <= 0.0f) {
		return 0.0f;
	}
	UVoxelLineTraceIterator* iter = NewObject<UVoxelLineTraceIterator>(this, UVoxelLineTraceIterator::StaticClass());
	iter->start = start / 100.0f;
	iter->direction = dir;
	while (true)
	{
		FIteratorReturn ret = iter->next();
		for (EDirection& dir : ret.out_voxel.direction) {
			auto* wall = WallObstucles.Find(FBoarderKey(dir, ret.out_voxel.voxel));
			if (wall) {
				return 0.0f;
			}
		}
		for (FVoxelResult& res : ret.in_voxels) {
			auto* solid = obstucles.Find(res.voxel);
			if (solid) {
				return 0.0f;
			}

			for (EDirection& dir : res.direction) {
				auto* wall = WallObstucles.Find(FBoarderKey(dir, res.voxel));
				if (wall) {
					return 0.0f;
				}
			}
			
			if (endTile == res.voxel) {
				return f;
			}
		}
	}
	return 0.0f;
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

void AWorldGrid::GetSpawnPoints() {
	BlueSpawnPoints.Empty();
	RedSpawnPoints.Empty();
	for (TActorIterator<AUnitSpawn> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AUnitSpawn* object = *ActorItr;
		switch (object->team)
		{
		case ETeam::T_Blue:
			BlueSpawnPoints.Add(object->origin);
			break;
		case ETeam::T_Red:
			RedSpawnPoints.Add(object->origin);
			break;
		default:
			break;
		}
	}
}

TArray<EDirection> AWorldGrid::directionFromIntVector(FIntVector dir)
{
	auto plah = [](int32 x, EDirection posDir, EDirection negDir, TArray<EDirection>& dirs) {
		if (x > 0) {
			dirs.Add(posDir);
		}
		else if (x < 0) {
			dirs.Add(negDir);
		}
	};
	TArray<EDirection> dirs = TArray<EDirection>();
	plah(dir.Y, EDirection::D_Backward, EDirection::D_Forward, dirs);
	plah(dir.X, EDirection::D_Rightward, EDirection::D_Leftward, dirs);
	plah(dir.Z, EDirection::D_Upward, EDirection::D_Downward, dirs);
	return dirs;
}

FIntVector AWorldGrid::directionToVector(EDirection dir)
{
	switch (dir) {
	case EDirection::D_Forward:
		return FIntVector(0, -1, 0);
	case EDirection::D_Backward:
		return FIntVector(0, 1, 0);
	case EDirection::D_Rightward:
		return FIntVector(1, 0, 0);
	case EDirection::D_Leftward:
		return FIntVector(-1, 0, 0);
	}
	return FIntVector(0, -1, 0);
}

EDirection AWorldGrid::addDirection(EDirection a, EDirection b)
{
	uint8 c = (uint8)a + (uint8)b;
	c = c % 4;
	return (EDirection)c;
}

EDirection AWorldGrid::oppersiteDirection(EDirection dir)
{
	switch (dir) {
	case EDirection::D_Forward:
		return EDirection::D_Backward;
	case EDirection::D_Backward:
		return EDirection::D_Forward;
	case EDirection::D_Rightward:
		return EDirection::D_Leftward;
	case EDirection::D_Leftward:
		return EDirection::D_Rightward;
	case EDirection::D_Upward:
		return EDirection::D_Downward;
	case EDirection::D_Downward:
		return EDirection::D_Upward;
	default:
		return EDirection::D_Forward;
	}
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
						if (currentScore < Limit && currentScore <  *x)
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
					UE_LOG(LogTemp, Verbose, TEXT("failed to find %d: %d"), current.X, current.Y);
				}
			}
		}
	}
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

bool AWorldGrid::isObstuclePresent(FIntVector pos, EDirection dir)
{
	FObstucle* obs = obstucles.Find(pos);
	if (obs) {
		return true;
	}
	obs = WallObstucles.Find(FBoarderKey(dir, pos));
	if (obs) {
		return true;
	}
	obs = WallObstucles.Find(FBoarderKey(dir, directionToVector(dir) + pos));
	if (obs) {
		return true;
	}
	return false;
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
	WallObstucles.Empty();

	TArray<ABlockingVolume*> GridBlockingVolumes;

	for (TActorIterator<ABlockingVolume> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ABlockingVolume* object = *ActorItr;
		GridBlockingVolumes.Push(object);
	}

	TArray<FGridTransform> trans;

	for (TActorIterator<AGridObject> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Access the subclass instance with the * or -> operators.
		AGridObject* object = *ActorItr;
		trans.Add(object->GridOrigin);
		AddBlockingTiles(object->GridOrigin);
		for (FGridTransform& GridOrigin : object->GridChildren) {
			FGridTransform go = GridOrigin;
			UE_LOG(LogWorld, Log, TEXT("starting postition %s"), *go.Origin.ToString());
			auto rot = FVector(directionToVector(object->GridOrigin.Direction) * -1).Rotation() + FRotator(0.0f, -90.0f, 0.0f);
			UE_LOG(LogWorld, Log, TEXT("rotation %s"), *rot.ToString());
			FVector fvec = rot.RotateVector(FVector(go.Origin));
			
			UE_LOG(LogWorld, Log, TEXT("FVector =  %s"), *fvec.ToString());
			go.Origin = FIntVector(FMath::RoundToInt(fvec.X), FMath::RoundToInt(fvec.Y), FMath::RoundToInt(fvec.Z));
			UE_LOG(LogWorld, Log, TEXT("relative postition %s, direction %s"), *go.Origin.ToString(), *directionToVector(object->GridOrigin.Direction).ToString());
			go.Origin = go.Origin + object->GridOrigin.Origin;
			UE_LOG(LogWorld, Log, TEXT("end postition %s"), *go.Origin.ToString());
			go.Direction = addDirection(go.Direction, object->GridOrigin.Direction);
			trans.Add(go);
			AddBlockingTiles(go);
		}
	}

	for (FGridTransform& tran : trans)
	{
		for (FIntVector& pos : tran.WalkablePosistions)
		{
			FIntVector loc = pos + tran.Origin;
			if (loc.X < minX) {
				minX = loc.X;
			}
			if (loc.Y < minY) {
				minY = loc.Y;
			}
			bool bInObstucle = obstucles.Contains(loc);
			bool bInBlockingVolume = checkBlockingVolume(loc, GridBlockingVolumes);
			if (!(bInObstucle || bInBlockingVolume)) {
				int32 x = loc.X, y = loc.Y, z = loc.Z;
				FVector vec = FVector((float)(x * spacing), (float)(y * spacing), (float)(z * spacing)) + FVector(spacing / 2, spacing / 2, 0.0f);
				/*
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
				*/
				gridTiles.Add(FIntVector(x, y, z), FGridTile());
			}
			else {
				UE_LOG(LogWorld, Verbose, TEXT("postition %sis blocked"), *pos.ToString());
			}
		}
	}

	GetSpawnPoints();

	Refresh = false;
	//Register all the components
	RegisterAllComponents();
}

bool AWorldGrid::checkBlockingVolume(FIntVector Pos, TArray<ABlockingVolume*> GridBlockingVolumes) {
	for (ABlockingVolume* vol : GridBlockingVolumes) {
		FVector max = vol->Brush->Bounds.Origin + vol->GetActorLocation() + vol->Brush->Bounds.BoxExtent;
		FVector min = vol->Brush->Bounds.Origin + vol->GetActorLocation() - vol->Brush->Bounds.BoxExtent;
		FVector pos = FVector(Pos * 100) + FVector(50.0f);
		if ((pos.X >= min.X && pos.Y >= min.Y && pos.Z >= min.Z) &&
			(pos.X <= max.X && pos.Y <= max.Y && pos.Z <= max.Z)) {
			return true;
		}
	}
	return false;
};

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

TArray<FIntVector> AWorldGrid::GetNeighbours(FIntVector origin, bool bReturnObstucles) {
	TArray<FIntVector> neighbours = TArray<FIntVector>();
	TArray<FIntVector> obstrucles = TArray<FIntVector>();

	auto opposite = [](EDirection dir) -> EDirection {
		switch (dir) {
		case EDirection::D_Forward:
			return EDirection::D_Backward;
		case EDirection::D_Backward:
			return EDirection::D_Forward;
		case EDirection::D_Rightward:
			return EDirection::D_Leftward;
		case EDirection::D_Leftward:
			return EDirection::D_Rightward;
		case EDirection::D_Upward:
			return EDirection::D_Downward;
		case EDirection::D_Downward:
			return EDirection::D_Upward;
		default:
			return EDirection::D_Forward;
		}
	};

	auto addDir = [](EDirection dir, FIntVector pos) -> FIntVector {
		switch (dir) {
		case EDirection::D_Forward:
			return pos + FIntVector(0,-1,0);
		case EDirection::D_Backward:
			return pos + FIntVector(0, 1, 0);
		case EDirection::D_Rightward:
			return pos + FIntVector(1, 0, 0);
		case EDirection::D_Leftward:
			return pos + FIntVector(-1, 1, 0);
		default:
			return pos;
		}
	};

	for (int32 x = -1; x < 2; x++) {
		for (int32 y = -1; y < 2; y++) {
			for (int32 z = -1; z < 2; z++) {
				if (!(x == 0 && y == 0)) {
					FIntVector dir = FIntVector(x, y, z);
					TArray<EDirection> odirs = directionFromIntVector(dir);
					FIntVector pos = dir + origin;
					if (gridTiles.Contains(pos)) {
						bool con = true;
						auto odirs = directionFromIntVector(dir);
						for (EDirection& odir: odirs) {
							if (WallObstucles.Contains(FBoarderKey(odir, origin))) {
								obstrucles.Add(origin);
								con = false;
							}
							if (WallObstucles.Contains(FBoarderKey(oppersiteDirection(odir), addDir(odir, origin)))) {
								obstrucles.Add(addDir(odir, origin));
								con = false;
							}
							auto s = obstucles.Find(addDir(odir, origin));
							if (s) {
								con = s->isUpToEdge ? false : con;
								obstrucles.Add(origin);
							}
						}
						if (con) {
							neighbours.Add(pos);
						}
					}
					else if (obstucles.Find(pos)) {
						obstrucles.Add(pos);
					}
				}
			}
		}
	}
	if (bReturnObstucles) {
		return obstrucles;
	}
	else {
		return neighbours;
	}
}

void AWorldGrid::AddBlockingTiles(FGridTransform GridOrigin)
{
	for (FIntVector& pos : GridOrigin.BlockedTiles) {
		FIntVector loc = pos + GridOrigin.Origin;
		if (GridOrigin.isBorder) {
			auto boarder = [](FIntVector loc, EDirection dir, EDirection righthand, TMap<FBoarderKey, FObstucle>& WallObstucles, FGridTransform GridOrigin) {
				WallObstucles.Add(FBoarderKey(dir, loc),
					FObstucle(GridOrigin.blockPercentage));
				if (GridOrigin.isRightHandCorner) {
					WallObstucles.Add(FBoarderKey(righthand, loc),
						FObstucle(GridOrigin.blockPercentage));
				};
			};
			switch (GridOrigin.Direction) {
			case EDirection::D_Forward:
				boarder(loc, EDirection::D_Forward, EDirection::D_Rightward, WallObstucles, GridOrigin);
				break;
			case EDirection::D_Rightward:
				boarder(loc, EDirection::D_Rightward, EDirection::D_Backward, WallObstucles, GridOrigin);
				break;
			case EDirection::D_Backward:
				boarder(loc, EDirection::D_Backward, EDirection::D_Leftward, WallObstucles, GridOrigin);
				break;
			case EDirection::D_Leftward:
				boarder(loc, EDirection::D_Leftward, EDirection::D_Forward, WallObstucles, GridOrigin);
				break;
			default:
				WallObstucles.Add(FBoarderKey(GridOrigin.Direction, loc),
					FObstucle(GridOrigin.blockPercentage));
				break;
			}
		}
		else
		{
			obstucles.Add(loc,
				FObstucle(GridOrigin.blockPercentage));
		}
	}
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