// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit.h"
#include "Mice.h"
#include "Classes/Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MPlayerController.h"
#include "MCommand.h"
#include "UnrealNetwork.h"
#include "Public/WorldGrid.h"
#include "../Public/Unit.h"

void AUnit::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUnit, team);
	DOREPLIFETIME(AUnit, pos);
	DOREPLIFETIME(AUnit, bHasExecutedCommand);
	DOREPLIFETIME(AUnit, command);
	DOREPLIFETIME(AUnit, weapon);
}

// Sets default values
AUnit::AUnit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	selectPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"));
	selectPlane->SetupAttachment(root);
	selectPlane->SetVisibility(false);

	bReplicates = true;
}

void AUnit::UpdatePos_Implementation(FIntVector n_pos)
{
	pos = n_pos;
}

bool AUnit::UpdatePos_Validate(FIntVector n_pos)
{
	return true;
}

float AUnit::CalculateProbabilityOfShot(FVector start, FVector end, bool bDebug)
{
	if (weapon.projectProbArc) {
		FVector dir = (end - start) / 100.0f;
		float f = weapon.projectProbArc->GetFloatValue(dir.Size());
		if (f <= 0.0f) {
			return 0.0f;
		}
		FCollisionQueryParams queryParams(FName(TEXT("FOW trace")), false, this);
		if (bDebug)
		{
			const FName TraceTag("TraceWeaponParams");
			GetWorld()->DebugDrawTraceTag = TraceTag;

			queryParams.TraceTag = TraceTag;
		}
		TArray<FHitResult> results;
		if (!GetWorld()->LineTraceMultiByChannel(results, start, end, ECC_GameTraceChannel3, queryParams)) {
			return f;
		}
		else {
			if (results[0].Distance >= (start - end).Size()) {
				return f;
			}
			return 0.0f;
		}
	}
	else {
		return 0.0f;
	}
	return 0.0f;
}
