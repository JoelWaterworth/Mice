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
/*
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
*/
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

TArray<FIntVector> AWorldGrid::directionFromIntVector(FIntVector dir)
{
	TArray<FIntVector> dirs;
	if (dir.X != 0) {
		dirs.Push(FIntVector(dir.X, 0, 0));
	}
	if (dir.Y != 0) {
		dirs.Push(FIntVector(0, dir.Y, 0));
	}
	if (dir.Z != 0) {
		dirs.Push(FIntVector(0, 0, dir.Z));
	}
	return dirs;
}

FIntVector AWorldGrid::directionToVector(EDirection dir)
{
	switch (dir) {
	case EDirection::D_Forward:
		FIntVector(0, 1, 0);
	case EDirection::D_Backward:
		FIntVector(0, -1, 0);
	case EDirection::D_Rightward:
		FIntVector(1, 0, 0);
	case EDirection::D_Leftward:
		FIntVector(-1, 0, 0);
	default:
		return FIntVector(0, 0, 0);
	}
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

bool AWorldGrid::isObstuclePresent(FIntVector origin, FIntVector dir)
{
	TArray<FIntVector> odirs = directionFromIntVector(dir);
	FIntVector pos = dir + origin;
	if (gridTiles.Contains(pos) && dir != FIntVector(0, 0, 0)) {
		if (obstucles.Contains(pos)) {
			return true;
		}
		for (int32 i = 0; i < odirs.Num(); i++) {
			FIntVector odir = odirs[i];
			if (WallObstucles.Contains(FBoarderKey(odir, pos))) {
				return true;
			}
			if (WallObstucles.Contains(FBoarderKey(odir * -1, origin))) {
				return true;
			}
		}
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
			auto rot = FVector(object->GridOrigin.Direction * -1).Rotation() + FRotator(0.0f, 90.0f, 0.0f);
			FVector fvec = rot.RotateVector(FVector(go.Origin));
			
			go.Origin = FIntVector(FMath::RoundToInt(fvec.X), FMath::RoundToInt(fvec.Y), FMath::RoundToInt(fvec.Z));
			go.Origin = go.Origin + object->GridOrigin.Origin;
			rot = FVector(object->GridOrigin.Direction * -1).Rotation() + FRotator(0.0f, 90.0f, 0.0f);
			FVector fdvec = rot.RotateVector(FVector(go.Direction));
			go.Direction = FIntVector(FMath::RoundToInt(fdvec.X), FMath::RoundToInt(fdvec.Y), FMath::RoundToInt(fdvec.Z));
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

	//for (int32 z = -1; z < 2; z++) {
	for (int32 y = -1; y < 2; y++) {
		for (int32 x = -1; x < 2; x++) {
			if (!(x == 0 && y == 0)) {
				FIntVector dir = FIntVector(x, y, origin.Z);
				TArray<FIntVector> odirs = directionFromIntVector(dir);
				FIntVector pos = dir + origin;
				if (gridTiles.Contains(pos) && dir != FIntVector(0,0,0)) {
					bool con = true;
					auto s = obstucles.Find(pos);
					if (s) {
						con = s->isUpToEdge ? false : con;
						obstrucles.Add(pos);
					}
					for (int32 i = 0; i < odirs.Num(); i++) {
						FIntVector odir = odirs[i];
						FBoarderKey key = FBoarderKey(odir, pos);
						if (WallObstucles.Contains(key)) {
							obstrucles.Add(pos);
							con = false;
						}
						else {
							UE_LOG(LogWorld, Warning, TEXT("dir=%s, postition=%s"), *odir.ToString(),*key.origin.ToString());
						}
						FBoarderKey okey = FBoarderKey(odir * -1, origin);
						if (WallObstucles.Contains(okey)) {
							obstrucles.Add(origin);
							con = false;
						}
						else {
							UE_LOG(LogWorld, Warning, TEXT("dir=%s, postition=%s"), *odir.ToString(), *okey.origin.ToString());
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
	//}
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
			auto boarder = [](FIntVector loc, FIntVector dir, TMap<FBoarderKey, FObstucle>& WallObstucles, FGridTransform GridOrigin) {
				WallObstucles.Add(FBoarderKey(dir, loc),
					FObstucle(GridOrigin.blockPercentage));
				if (GridOrigin.isRightHandCorner) {
					FVector fvec = FRotator(0.0f, 0.0f, 0.0f).RotateVector(FVector(dir));
					auto righthand = FIntVector(FMath::RoundToInt(fvec.X), FMath::RoundToInt(fvec.Y), FMath::RoundToInt(fvec.Z));
					WallObstucles.Add(FBoarderKey(righthand, loc),
						FObstucle(GridOrigin.blockPercentage));
				};
			};
			boarder(loc, GridOrigin.Direction, WallObstucles, GridOrigin);
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