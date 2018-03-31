// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MPlayerState.h"
#include "Classes/Sound/SoundCue.h"
#include "GameFramework/Character.h"
#include "Unit.generated.h"

USTRUCT(BlueprintType)
struct FWeapon
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCurveFloat* projectProbArc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundCue* sound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 minDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 maxDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 maxAmmoCount;
public:
	FWeapon(
		UCurveFloat* arc = nullptr,
		USoundCue* s = nullptr,
		int32 minD = 6,
		int32 maxD = 8,
		int32 maxAmmo = 8) :
			projectProbArc(arc),
			sound(s),
			minDamage(minD),
			maxDamage(maxD),
			maxAmmoCount(maxAmmo){}
};

UENUM(BlueprintType)
enum class EUnitState : uint8
{
	US_Idle 		UMETA(DisplayName = "Idle"),
	US_Walk 		UMETA(DisplayName = "Walk"),
	US_Shooting		UMETA(DisplayName = "Shooting"),
	US_Overwatch	UMETA(DisplayName = "Overwatch"),
};

class UMCommand;

class AMPlayerController;

UCLASS(BlueprintType, Blueprintable)
class MICE_API AUnit : public ACharacter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUnit();

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Mesh)
		UStaticMeshComponent* selectPlane;

	UPROPERTY(BlueprintReadWrite)
		TMap<FIntVector, FIntVector> roots;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Team")
		ETeam team;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Team")
		FWeapon weapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float height = 150.0f;
	UPROPERTY(EditAnywhere)
		bool isActorInTerraIncog = false;

protected:

	USceneComponent* root;

public:
	UFUNCTION(BlueprintCallable)
		float CalculateProbabilityOfShot(FVector start, FVector end, bool bDebug = false);

	UFUNCTION(server, reliable, WithValidation, BlueprintCallable)
		void UpdatePos(FIntVector n_pos);

	UPROPERTY(Replicated, BlueprintReadWrite)
		FIntVector pos;
	UPROPERTY(Replicated, BlueprintReadWrite)
		bool bHasExecutedCommand;
	UPROPERTY(Replicated, BlueprintReadWrite)
		UMCommand* command;
};
